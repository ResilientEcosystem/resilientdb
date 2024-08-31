#include "benchmark/pbft/benchmark_server_impl.h"

#include <glog/logging.h>

#include "benchmark/pbft/proto/benchmark.pb.h"

namespace resdb {

BenchmarkServerImpl::BenchmarkServerImpl() {}
std::unique_ptr<std::string> BenchmarkServerImpl::ExecuteData(
    const std::string& request) {
  BenchmarkResponse response;
  response.set_data(std::string('b', 17));
  std::unique_ptr<std::string> resp = std::make_unique<std::string>();
  response.SerializeToString(resp.get());
  return resp;
}

}  // namespace resdb
