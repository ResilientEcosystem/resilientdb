#include "kv_server/kv_server_executor.h"

#include <glog/logging.h>

#include "proto/kv_server.pb.h"

namespace resdb {

std::unique_ptr<std::string> KVServerExecutor::ExecuteData(
    const std::string& request) {
  KVRequest kv_request;
  KVResponse kv_response;

  if (!kv_request.ParseFromString(request)) {
    LOG(ERROR) << "parse data fail";
    return nullptr;
  }

  if (kv_request.cmd() == KVRequest::SET) {
    Set(kv_request.key(), kv_request.value());
  } else {
    kv_response.set_value(Get(kv_request.key()));
  }

  std::unique_ptr<std::string> resp_str = std::make_unique<std::string>();
  if (!kv_response.SerializeToString(resp_str.get())) {
    return nullptr;
  }

  return resp_str;
}

void KVServerExecutor::Set(const std::string& key, const std::string& value) {
  LOG(ERROR) << "set value:" << key << " " << value;
  kv_map_[key] = value;
}

std::string KVServerExecutor::Get(const std::string& key) {
  LOG(ERROR) << "get value:" << key << " " << kv_map_[key];
  return kv_map_[key];
}

}  // namespace resdb
