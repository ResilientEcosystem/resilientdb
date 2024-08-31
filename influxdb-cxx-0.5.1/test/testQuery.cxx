#define BOOST_TEST_MODULE Test InfluxDB Query
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../include/InfluxDBFactory.h"
#include "../src/InfluxDBException.h"

namespace influxdb {
namespace test {



BOOST_AUTO_TEST_CASE(query1)
{
  auto influxdb = influxdb::InfluxDBFactory::Get("http://localhost:8086?db=test");
  auto points = influxdb->query("SELECT * from test WHERE host = 'localhost' LIMIT 3");
  BOOST_CHECK_EQUAL(points.size(), 3);
  BOOST_CHECK_EQUAL(points[0].getName(), "test");
  BOOST_CHECK_EQUAL(points[1].getName(), "test");
  BOOST_CHECK_EQUAL(points[2].getName(), "test");
  BOOST_CHECK_EQUAL(points[0].getFields(), "value=10");
  BOOST_CHECK_EQUAL(points[1].getFields(), "value=20");
  BOOST_CHECK_EQUAL(points[2].getFields(), "value=200");
  BOOST_CHECK_EQUAL(points[0].getTags(), "host=localhost");
  BOOST_CHECK_EQUAL(points[1].getTags(), "host=localhost");
  BOOST_CHECK_EQUAL(points[2].getTags(), "host=localhost");
}

BOOST_AUTO_TEST_CASE(timeStampVerify)
{
  double timeZone = 3600; //+1h

  auto influxdb = influxdb::InfluxDBFactory::Get("http://localhost:8086?db=test");
  Point point = Point{"timestampCheck"}.addField("value", 10);
  auto timestamp = point.getTimestamp();
  influxdb->write(std::move(point));

  auto points = influxdb->query("SELECT * from timestampCheck ORDER BY DESC LIMIT 1");
  std::chrono::duration<double> diff = timestamp - points[0].getTimestamp();
  double diffZone = diff.count() - timeZone;
  BOOST_CHECK(diffZone < 1); // 1s
}

BOOST_AUTO_TEST_CASE(failedQuery1)
{
  auto influxdb = influxdb::InfluxDBFactory::Get("http://localhost:8086?db=test");
  auto points = influxdb->query("SELECT * from test1 WHERE host = 'localhost' LIMIT 3");
  BOOST_CHECK_EQUAL(points.size(), 0);
}

BOOST_AUTO_TEST_CASE(failedQuery2)
{
  auto influxdb = influxdb::InfluxDBFactory::Get("http://localhost:8086?db=test");
  BOOST_CHECK_THROW(influxdb->query("SELECT *from test1 WHEREhost = 'localhost' LIMIT 3"), InfluxDBException);
}

} // namespace test
} // namespace influxdb
