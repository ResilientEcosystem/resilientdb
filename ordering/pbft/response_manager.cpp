#include "ordering/pbft/response_manager.h"

#include <glog/logging.h>

#include "common/utils/utils.h"

namespace resdb {
ResponseManager::ResponseManager(const ResDBConfig& config,
                                 ResDBReplicaClient* replica_client,
                                 SystemInfo* system_info)
    : config_(config),
      replica_client_(replica_client),
      collector_pool_(std::make_unique<LockFreeCollectorPool>(
          "response", config_.GetMaxProcessTxn(), nullptr)),
      context_pool_(std::make_unique<LockFreeCollectorPool>(
          "context", config_.GetMaxProcessTxn(), nullptr)),
      batch_queue_("client request", config_.ClientBatchNum()),
      system_info_(system_info) {
  stop_ = false;
  local_id_ = 1;
  client_req_thread_ = std::thread(&ResponseManager::BatchProposeMsg, this);
  global_stats_ = Stats::GetGlobalStats();
}

ResponseManager::~ResponseManager() {
  stop_ = true;
  if (client_req_thread_.joinable()) {
    client_req_thread_.join();
  }
}

// use system info
int ResponseManager::GetPrimary() { return system_info_->GetPrimaryId(); }

int ResponseManager::AddContextList(
    std::vector<std::unique_ptr<Context>> context_list, uint64_t id) {
  return context_pool_->GetCollector(id)->SetContextList(
      id, std::move(context_list));
}

std::vector<std::unique_ptr<Context>> ResponseManager::FetchContextList(
    uint64_t id) {
  auto context = context_pool_->GetCollector(id)->FetchContextList(id);
  context_pool_->Update(id);
  return context;
}

int ResponseManager::NewClientRequest(std::unique_ptr<Context> context,
                                      std::unique_ptr<Request> client_request) {
  if (!client_request->need_response()) {
    context->client = nullptr;
  }

  std::unique_ptr<QueueItem> queue_item = std::make_unique<QueueItem>();
  queue_item->context = std::move(context);
  queue_item->client_request = std::move(client_request);

  batch_queue_.Push(std::move(queue_item));
  return 0;
}

// =================== response ========================
// handle the response message. If receive f+1 commit messages, send back to the
// client.
int ResponseManager::ProcessResponseMsg(std::unique_ptr<Context> context,
                                        std::unique_ptr<Request> request) {
  std::unique_ptr<Request> response;
  // Add the response message, and use the call back to collect the received
  // messages.
  // The callback will be triggered if it received f+1 messages.
  CollectorResultCode ret =
      AddResponseMsg(context->signature, std::move(request),
                     [&](const Request& request,
                         const TransactionCollector::CollectorDataType*) {
                       response = std::make_unique<Request>(request);
                       return;
                     });

  if (ret == CollectorResultCode::STATE_CHANGED) {
    BatchClientResponse batch_response;
    if (batch_response.ParseFromString(response->data())) {
      SendResponseToClient(batch_response);
    } else {
      LOG(ERROR) << "parse response fail:";
    }
  }
  return ret == CollectorResultCode::INVALID ? -2 : 0;
}

bool ResponseManager::MayConsensusChangeStatus(
    int type, int received_count, std::atomic<TransactionStatue>* status) {
  switch (type) {
    case Request::TYPE_RESPONSE:
      // if receive f+1 response results, ack to the client.
      if (*status == TransactionStatue::None &&
          config_.GetMinClientReceiveNum() <= received_count) {
        TransactionStatue old_status = TransactionStatue::None;
        return status->compare_exchange_strong(
            old_status, TransactionStatue::EXECUTED, std::memory_order_acq_rel,
            std::memory_order_acq_rel);
      }
      break;
  }
  return false;
}

CollectorResultCode ResponseManager::AddResponseMsg(
    const SignatureInfo& signature, std::unique_ptr<Request> request,
    std::function<void(const Request&,
                       const TransactionCollector::CollectorDataType*)>
        response_call_back) {
  if (request == nullptr) {
    return CollectorResultCode::INVALID;
  }

  int type = request->type();
  uint64_t seq = request->seq();
  int resp_received_count = 0;
  int ret = collector_pool_->GetCollector(seq)->AddRequest(
      std::move(request), signature, false,
      [&](const Request& request, int received_count,
          TransactionCollector::CollectorDataType* data,
          std::atomic<TransactionStatue>* status) {
        if (MayConsensusChangeStatus(type, received_count, status)) {
          resp_received_count = 1;
          response_call_back(request, data);
        }
      });
  if (ret != 0) {
    return CollectorResultCode::INVALID;
  }
  if (resp_received_count > 0) {
    collector_pool_->Update(seq);
    return CollectorResultCode::STATE_CHANGED;
  }
  return CollectorResultCode::OK;
}

void ResponseManager::SendResponseToClient(
    const BatchClientResponse& batch_response) {
  uint64_t create_time = batch_response.createtime();
  uint64_t local_id = batch_response.local_id();
  LOG(ERROR) << " send to client";
  if (create_time > 0) {
    uint64_t run_time = get_sys_clock() - create_time;
    global_stats_->AddLatency(run_time);
  } else {
    LOG(ERROR) << "seq:" << local_id << " no resp";
  }

  if (config_.IsPerformanceRunning()) {
    return;
  }

  std::vector<std::unique_ptr<Context>> context_list =
      FetchContextList(batch_response.local_id());
  if (context_list.empty()) {
    LOG(ERROR) << "context list is empty. local id:"
               << batch_response.local_id();
    return;
  }

  for (size_t i = 0; i < context_list.size(); ++i) {
    auto& context = context_list[i];
    if (context->client == nullptr) {
      LOG(ERROR) << " no client:";
      continue;
    }
    int ret = context->client->SendRawMessageData(batch_response.response(i));
    if (ret) {
      LOG(ERROR) << "send resp fail ret:" << ret;
    }
  }
}

// =================== request ========================
int ResponseManager::BatchProposeMsg() {
  LOG(ERROR) << "batch wait time?:" << config_.ClientBatchWaitTimeMS()
             << " batch num:" << config_.ClientBatchNum();
  while (!stop_) {
    std::vector<std::unique_ptr<QueueItem>> batch_req =
        batch_queue_.Pop(config_.ClientBatchWaitTimeMS());
    if (batch_req.empty()) {
      continue;
    }
    int ret = DoBatch(batch_req);
    if (ret != 0) {
      Response response;
      response.set_result(Response::ERROR);
      for (size_t i = 0; i < batch_req.size(); ++i) {
        if (batch_req[i]->context && batch_req[i]->context->client) {
          int ret = batch_req[i]->context->client->SendRawMessage(response);
          if (ret) {
            LOG(ERROR) << "send resp" << response.DebugString()
                       << " fail ret:" << ret;
          }
        }
      }
    }
    // usleep(10);
  }
  return 0;
}

int ResponseManager::DoBatch(
    const std::vector<std::unique_ptr<QueueItem>>& batch_req) {
  auto new_request =
      NewRequest(Request::TYPE_NEW_TXNS, Request(), config_.GetSelfInfo().id());
  if (new_request == nullptr) {
    return -2;
  }
  std::vector<std::unique_ptr<Context>> context_list;

  BatchClientRequest batch_request;
  for (size_t i = 0; i < batch_req.size(); ++i) {
    BatchClientRequest::ClientRequest* req =
        batch_request.add_client_requests();
    req->mutable_request()->Swap(batch_req[i]->client_request.get());
    *req->mutable_signature() = batch_req[i]->context->signature;
    req->set_id(i);
    context_list.push_back(std::move(batch_req[i]->context));
  }
  LOG(ERROR)<<"get client batch:"<<batch_req.size();

  if (!config_.IsPerformanceRunning()) {
    LOG(ERROR) << "add context list:" << new_request->seq()
               << " list size:" << context_list.size();
    batch_request.set_local_id(local_id_);
    int ret = AddContextList(std::move(context_list), local_id_++);
    if (ret != 0) {
      LOG(ERROR) << "add context list fail:";
      return ret;
    }
  }

  batch_request.set_createtime(get_sys_clock());
  batch_request.SerializeToString(new_request->mutable_data());
  new_request->set_hash(SignatureVerifier::CalculateHash(new_request->data()));
  new_request->set_proxy_id(config_.GetSelfInfo().id());
  replica_client_->SendMessage(*new_request, GetPrimary());
  return 0;
}

}  // namespace resdb
