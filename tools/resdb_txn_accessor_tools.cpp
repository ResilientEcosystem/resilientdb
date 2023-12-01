/*
 * Copyright (c) 2019-2022 ExpoLab, UC Davis
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <glog/logging.h>

#include "interface/common/resdb_txn_accessor.h"
#include "platform/config/resdb_config_utils.h"

using namespace resdb;

int main(int argc, char** argv) {
  if (argc < 4) {
    printf(
        "<config path> <min_seq> <max_seq>\n");
    return 0;
  }
  std::string config_file = argv[1];
  uint64_t min_seq = atoi(argv[2]);
  uint64_t max_seq = atoi(argv[3]);

  ReplicaInfo self_info = GenerateReplicaInfo(0, "127.0.0.1", 88888);

  ResDBConfig config = GenerateResDBConfig(config_file);

  ResDBTxnAccessor client(config);
  auto block_or = client.GetBlockNumbers();
  if(!block_or.ok()){
    LOG(ERROR)<<"get number fail";
    exit(1);
  }
  LOG(ERROR)<<"total block:"<<*block_or;

  auto resp = client.GetTxn(min_seq, max_seq);
  absl::StatusOr<std::vector<std::pair<uint64_t, std::string>>> GetTxn(
      uint64_t min_seq, uint64_t max_seq);
  if (!resp.ok()) {
    LOG(ERROR) << "get replica state fail";
    exit(1);
  }
  for (auto& txn : *resp) {
    LOG(ERROR) << "seq:" << txn.first << " txn:" << txn.second;
  }
}
