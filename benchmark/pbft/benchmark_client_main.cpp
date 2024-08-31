#include <glog/logging.h>

#include "benchmark/pbft/benchmark_client.h"
#include "common/utils/utils.h"
#include "config/resdb_config_utils.h"

using resdb::BenchmarkClient;
using resdb::GenerateResDBConfig;
using resdb::get_sys_clock;
using resdb::ReplicaInfo;
using resdb::ResDBConfig;

int main(int argc, char** argv) {
  if (argc < 5) {
    printf("<config path> <data_size> <request num> <thread num> \n");
    return 0;
  }
  std::string config_file = argv[1];
  int value_size = std::stoi(argv[2]);

  ResDBConfig config = GenerateResDBConfig(config_file);

  config.SetClientTimeoutMs(10000000);
  BenchmarkClient client(config);
  int ret = client.Set(std::string(value_size, 't'));
  if (ret != 0) {
    printf("client set fail ret = %d\n", ret);
  }
}
