#include "application/poc/pbft_client.h"

#include "ordering/poc/proto/transaction.pb.h"

namespace resdb {

PBFTClient::PBFTClient(const ResDBConfig& config) : ResDBUserClient(config) {}

int PBFTClient::Set(const std::string& data) {
  Transaction txn;
  txn.set_data(data);
  return SendRequest(txn);
}

}  // namespace resdb
