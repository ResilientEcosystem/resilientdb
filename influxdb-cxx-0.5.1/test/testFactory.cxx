#define BOOST_TEST_MODULE Test InfluxDB Factory
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/InfluxDBFactory.h"
#include "../src/InfluxDBException.h"

namespace influxdb {
namespace test {


BOOST_AUTO_TEST_CASE(unrecognisedBackend)
{
  BOOST_CHECK_THROW(influxdb::InfluxDBFactory::Get("httpz://localhost:8086?db=test"), InfluxDBException);
}

BOOST_AUTO_TEST_CASE(missformatedUrl)
{
  BOOST_CHECK_THROW(influxdb::InfluxDBFactory::Get("localhost:8086?db=test"), InfluxDBException);
}

BOOST_AUTO_TEST_CASE(missingDb)
{
  BOOST_CHECK_THROW(influxdb::InfluxDBFactory::Get("http://localhost:8086"), InfluxDBException);
}

} // namespace test
} // namespace influxdb
