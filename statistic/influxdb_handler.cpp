#include "statistic/infludb_handler.h"

#include <glog/logging.h>

namespace resdb {

using prometheus::Histogram;

InfluxdbHandler::InfluxdbHandler(const std::string& server_address) {
}

InfluxdbHandler::~InfluxdbHandler() {
}

void InfluxdbHandler::IncQPS() {
    ret = influxdb_cpp::builder()
        .meas("test")
        .tag("k", "v")
        .tag("x", "y")
        .field("x", 10)
        .field("y", 3.14e18, 3)
        .field("b", !!10)
        .timestamp(1512722735522840439)
        .send_udp("127.0.0.1", 8089);
}

}  // namespace resdb
