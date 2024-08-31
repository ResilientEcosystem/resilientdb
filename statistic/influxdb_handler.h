#pragma once

#include <glog/logging.h>
#include "statistic/influxdb.hpp"


namespace resdb {

class InfluxdbHandler {
 public:
  InfluxdbHandler(const std::string& server_address);
  ~InfluxdbHandler();

  void IncQPS();
};

}  // namespace resdb
