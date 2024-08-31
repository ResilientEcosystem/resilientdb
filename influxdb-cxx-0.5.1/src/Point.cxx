///
/// \author Adam Wegrzynek <adam.wegrzynek@cern.ch>
///

#include "Point.h"

#include <iostream>
#include <chrono>
#include <memory>
#include <sstream>

namespace influxdb
{

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Point::Point(const std::string& measurement) :
  mMeasurement(measurement), mTimestamp(Point::getCurrentTimestamp())
{
  mValue = {};
  mTags = {};
  mFields = {};
}

Point&& Point::addField(std::string_view name, std::variant<int, long long int, std::string, double> value)
{
  std::stringstream convert;
  if (!mFields.empty()) convert << ",";

  convert << name << "=";
  std::visit(overloaded {
    [&convert](int value) { convert << value << 'i'; },
    [&convert](long long int value) { convert << value << 'i'; },
    [&convert](double value) { convert << value; },
    [&convert](const std::string& value) { convert << '"' << value << '"'; },
    }, value);
  mFields += convert.str();
  return std::move(*this);
}

Point&& Point::addTag(std::string_view key, std::string_view value)
{
  mTags += ",";
  mTags += key;
  mTags += "=";
  mTags += value;
  return std::move(*this);
}

Point&& Point::setTimestamp(std::chrono::time_point<std::chrono::system_clock> timestamp)
{
  mTimestamp = timestamp;
  return std::move(*this);
}

auto Point::getCurrentTimestamp() -> decltype(std::chrono::system_clock::now())
{
  return std::chrono::system_clock::now();
}

std::string Point::toLineProtocol() const
{
  return mMeasurement + mTags + " " + mFields + " " + std::to_string(
    std::chrono::duration_cast <std::chrono::nanoseconds>(mTimestamp.time_since_epoch()).count()
  );
}

std::string Point::getName() const
{
  return mMeasurement;
}

std::chrono::time_point<std::chrono::system_clock> Point::getTimestamp() const
{
  return mTimestamp;
}

std::string Point::getFields() const
{
  return mFields;
}

std::string Point::getTags() const
{
  return mTags.substr(1, mTags.size());
}

} // namespace influxdb
