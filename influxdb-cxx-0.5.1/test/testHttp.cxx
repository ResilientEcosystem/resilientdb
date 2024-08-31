#define BOOST_TEST_MODULE Test InfluxDB HTTP
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/InfluxDBFactory.h"
#include "../src/InfluxDBException.h"

namespace influxdb {
namespace test {

BOOST_AUTO_TEST_CASE(write1)
{
  auto influxdb = influxdb::InfluxDBFactory::Get("http://localhost:8086?db=test");
  influxdb->write(Point{"test"}
    .addField("value", 10)
    .addTag("host", "localhost")
 );

  influxdb->write(Point{"test"}
    .addField("value", 20)
    .addTag("host", "localhost")
  );

  influxdb->write(Point{"test"}
    .addField("value", 200LL)
    .addTag("host", "localhost"));

    influxdb->write(Point{"string"}
    .addField("value", "influxdb-cxx")
    .addTag("host", "localhost"));
}

BOOST_AUTO_TEST_CASE(writeWrongHost)
{
  auto influxdb = influxdb::InfluxDBFactory::Get("http://localhost2:8086?db=test");
  BOOST_CHECK_THROW(influxdb->write(Point{"test"}.addField("value", 10)), InfluxDBException);
}

} // namespace test
} // namespace influxdb
