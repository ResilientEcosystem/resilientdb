#include "application/poc/pbft_client.h"
#include "config/resdb_config_utils.h"

using resdb::GenerateResDBConfig;
using resdb::PBFTClient;
using resdb::ResDBConfig;

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("<config path> data\n");
    return 0;
  }
  std::string config_file = argv[1];
  std::string value = argv[2];

  ResDBConfig config = GenerateResDBConfig(config_file);

  PBFTClient client(config);
  int ret = client.Set(value);
  printf("client set ret = %d\n", ret);
}
