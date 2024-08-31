#include "application/utils/server_factory.h"
#include "benchmark/pbft/benchmark_server_impl.h"
#include "statistic/stats.h"

using resdb::BenchmarkServerImpl;
using resdb::GenerateResDBServer;
using resdb::ResDBConfig;
using resdb::Stats;

void ShowUsage() { printf("<config> <private_key> <cert_file>\n"); }

int main(int argc, char** argv) {
  if (argc < 3) {
    ShowUsage();
    exit(0);
  }

  char* config_file = argv[1];
  char* private_key_file = argv[2];
  char* cert_file = argv[3];

  Stats::GetGlobalStats(/*int sleep_seconds = */ 5);

  auto server = GenerateResDBServer(
      config_file, private_key_file, cert_file,
      std::make_unique<BenchmarkServerImpl>(), nullptr,
      [&](ResDBConfig* config) { config->RunningPerformance(true); });

  server->Run();
}
