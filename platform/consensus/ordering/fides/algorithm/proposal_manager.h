#pragma once

#include "platform/consensus/ordering/fides/proto/proposal.pb.h"
#include "enclave/sgx_cpp_u.h"

namespace resdb {
namespace fides {

class ProposalManager {
 public:
  ProposalManager(int32_t id, int limit_count, oe_enclave_t* enclave);

  std::unique_ptr<Proposal> GenerateProposal(
      const std::vector<std::unique_ptr<Transaction>>& txns);

  int CurrentRound();
  void AddLocalBlock(std::unique_ptr<Proposal> proposal);
  const Proposal* GetLocalBlock(const std::string& hash);
  std::unique_ptr<Proposal> FetchLocalBlock(const std::string& hash);

  void AddBlock(std::unique_ptr<Proposal> proposal);
  void AddCert(std::unique_ptr<Certificate> cert);
  bool Ready();

  const Proposal* GetRequest(int round, int sender);
  std::unique_ptr<Proposal> FetchRequest(int round, int sender);
  int GetReferenceNum(const Proposal& req);

  bool VerifyHash(const Proposal &proposal);

  bool CheckCert(int round, int sender);
  bool CheckBlock(const std::string& hash);

  void SetCommittedRound(int r);

  std::vector<CounterInfo> GetCounterFromRound(int round);

 protected:
  void GetMetaData(Proposal* proposal);

 private:
  int32_t id_;
  int round_;
  int limit_count_;
  std::map<std::string, std::unique_ptr<Proposal>> block_, local_block_;

  std::map<int64_t, std::map<int, std::unique_ptr<Certificate>>> cert_list_;
  std::map<int, std::unique_ptr<Certificate>> latest_cert_from_sender_;

  std::mutex txn_mutex_, local_mutex_;
  std::map<std::pair<int, int>, std::vector<int>> reference_;

  std::atomic<int> committed_round_ = 0;

  oe_enclave_t* enclave_;
  oe_result_t result;
};

}  // namespace fides
}  // namespace resdb
