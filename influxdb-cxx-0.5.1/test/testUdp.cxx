#define BOOST_TEST_MODULE Test InfluxDB UDP
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/InfluxDBFactory.h"

namespace influxdb {
namespace test {

BOOST_AUTO_TEST_CASE(test)
{
  auto influxdb = influxdb::InfluxDBFactory::Get("udp://localhost:8084");
  influxdb->write(Point{"test"}
    .addField("value", 10)
    .addField("value", 20)
    .addField("value", 100LL)
    .addTag("host", "adampc")
  );
}

BOOST_AUTO_TEST_CASE(test2)
{
  auto influxdb = influxdb::InfluxDBFactory::Get("udp://localhost:8084");
  influxdb->batchOf(2);
  influxdb->write(Point{"test"}.addField("value", 10));
  influxdb->write(Point{"test"}.addField("value", 10));
  influxdb->write(Point{"test"}.addField("value", 10));
  influxdb->write(Point{"test"}.addField("value", 100LL));
  influxdb->write(Point{"test"}.addField("value", 100LL));
  influxdb->write(Point{"test"}.addField("value", 100LL));
}

} // namespace test
} // namespace influxdb
