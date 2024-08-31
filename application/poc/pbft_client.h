#pragma once

#include "client/resdb_user_client.h"

namespace resdb {

// PBFTClient to send data to the pbft cluster.
class PBFTClient : public ResDBUserClient {
 public:
  PBFTClient(const ResDBConfig& config);

  int Set(const std::string& data);
};

}  // namespace resdb
