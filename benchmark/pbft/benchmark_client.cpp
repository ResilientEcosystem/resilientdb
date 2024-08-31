#include "benchmark/pbft/benchmark_client.h"

#include <glog/logging.h>

#include "benchmark/pbft/proto/benchmark.pb.h"

namespace resdb {

BenchmarkClient::BenchmarkClient(const ResDBConfig& config)
    : ResDBUserClient(config) {}

int BenchmarkClient::Set(const std::string& data) {
  BenchmarkMessage request;
  request.set_data(data);
  return SendRequest(request);
  //  BenchmarkResponse response;
  //  return SendRequest(request, &response);
}

}  // namespace resdb
