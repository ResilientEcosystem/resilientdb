///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "InfluxDB.h"
#include "InfluxDBException.h"

#include <iostream>
#include <memory>
#include <string>

#ifdef INFLUXDB_WITH_BOOST
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#endif

namespace influxdb
{

InfluxDB::InfluxDB(std::unique_ptr<Transport> transport) :
  mTransport(std::move(transport))
{
  mBuffer = {};
  mBuffering = false;
  mBufferSize = 0;
  mGlobalTags = {};
}

void InfluxDB::batchOf(const std::size_t size)
{
  mBufferSize = size;
  mBuffering = true;
}

void InfluxDB::flushBuffer() {
  if (!mBuffering || mBuffer.empty()) {
    return;
  }
  std::string stringBuffer{};
  for (const auto &i : mBuffer) {
    stringBuffer+= i + "\n";
  }
  mBuffer.clear();
  transmit(std::move(stringBuffer));
}

void InfluxDB::addGlobalTag(std::string_view key, std::string_view value)
{
  if (!mGlobalTags.empty()) mGlobalTags += ",";
  mGlobalTags += key;
  mGlobalTags += "=";
  mGlobalTags += value;
}

InfluxDB::~InfluxDB()
{
  if (mBuffering) {
    flushBuffer();
  }
}

void InfluxDB::transmit(std::string&& point)
{
  mTransport->send(std::move(point));
}

void InfluxDB::write(Point&& metric)
{
  if (mBuffering) {
    mBuffer.emplace_back(metric.toLineProtocol());
    if (mBuffer.size() >= mBufferSize) {
      flushBuffer();
    }
  } else {
    transmit(metric.toLineProtocol());
  }
}

#ifdef INFLUXDB_WITH_BOOST
std::vector<Point> InfluxDB::query(const std::string&  query)
{
  auto response = mTransport->query(query);
  std::stringstream ss;
  ss << response;
  std::vector<Point> points;
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);

  for (auto& result : pt.get_child("results")) {
    auto isResultEmpty = result.second.find("series");
    if (isResultEmpty == result.second.not_found()) return {};
    for (auto& series : result.second.get_child("series")) {
      auto columns = series.second.get_child("columns");

      for (auto& values : series.second.get_child("values")) {
        Point point{series.second.get<std::string>("name")};
        auto iColumns = columns.begin();
        auto iValues = values.second.begin();
        for (; iColumns != columns.end() && iValues != values.second.end(); iColumns++, iValues++) {
          auto value = iValues->second.get_value<std::string>();
          auto column = iColumns->second.get_value<std::string>();
          if (!column.compare("time")) {
            std::tm tm = {};
            std::stringstream ss;
            ss << value;
            ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
            point.setTimestamp(std::chrono::system_clock::from_time_t(std::mktime(&tm)));
            continue;
          }
          // cast all values to double, if strings add to tags
          try { point.addField(column, boost::lexical_cast<double>(value)); }
          catch(...) { point.addTag(column, value); }
        }
        points.push_back(std::move(point));
      }
    }
  }
  return points;
}
#else
std::vector<Point> InfluxDB::query(const std::string& /*query*/)
{
  throw InfluxDBException("InfluxDB::query", "Boost is required");
}
#endif

} // namespace influxdb
