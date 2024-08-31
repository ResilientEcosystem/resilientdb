#include <glog/logging.h>

#include "application/utils/server_factory.h"
#include "kv_server/kv_server_executor.h"

using resdb::KVServerExecutor;
using resdb::ResDBServer;

void ShowUsage() {
  printf("<config> <private_key> <cert_file> [logging_dir]\n");
}

int main(int argc, char** argv) {
  if (argc < 3) {
    ShowUsage();
    exit(0);
  }

  char* config_file = argv[1];
  char* private_key_file = argv[2];
  char* cert_file = argv[3];
  char* logging_dir = nullptr;
  if (argc >= 5) {
    logging_dir = argv[4];
  }

  auto server =
      GenerateResDBServer(config_file, private_key_file, cert_file,
                          std::make_unique<KVServerExecutor>(), logging_dir);
  server->Run();
}
