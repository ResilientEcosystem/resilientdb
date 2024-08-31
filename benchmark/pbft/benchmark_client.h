#pragma once

#include "client/resdb_user_client.h"

namespace resdb {

// BenchmarkClient to send data to the pbft cluster.
class BenchmarkClient : public ResDBUserClient {
 public:
  BenchmarkClient(const ResDBConfig& config);

  int Set(const std::string& data);
};

}  // namespace resdb
