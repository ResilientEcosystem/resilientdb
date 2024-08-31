#include "config/resdb_config_utils.h"
#include "ordering/pbft/consensus_service_pbft.h"
#include "server/resdb_server.h"

using resdb::ConsensusServicePBFT;
using resdb::GenerateResDBConfig;
using resdb::ResDBConfig;
using resdb::ResDBServer;

void ShowUsage() { printf("<config> <private_key> <cert_file>\n"); }

int main(int argc, char** argv) {
  if (argc < 3) {
    ShowUsage();
    exit(0);
  }

  char* config_file = argv[1];
  char* private_key_file = argv[2];
  char* cert_file = argv[3];

  std::unique_ptr<ResDBConfig> config =
      GenerateResDBConfig(config_file, private_key_file, cert_file);

  ResDBServer server(*config,
                     std::make_unique<ConsensusServicePBFT>(*config, nullptr));
  server.Run();
}

