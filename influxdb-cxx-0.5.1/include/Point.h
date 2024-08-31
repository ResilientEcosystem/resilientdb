///
/// \author Adam Wegrzynek
///

#ifndef INFLUXDATA_POINT_H
#define INFLUXDATA_POINT_H

#include <string>
#include <chrono>
#include <variant>

namespace influxdb
{

/// \brief Represents a point
class Point
{
  public:
    /// Constructs point based on measurement name
    Point(const std::string& measurement);

    /// Default destructor
    ~Point() = default;

    /// Adds a tags
    Point&& addTag(std::string_view key, std::string_view value);

    /// Adds filed
    Point&& addField(std::string_view name, std::variant<int, long long int, std::string, double> value);

    /// Generetes current timestamp
    static auto getCurrentTimestamp() -> decltype(std::chrono::system_clock::now());

    /// Converts point to Influx Line Protocol
    std::string toLineProtocol() const;

    /// Sets custom timestamp
    Point&& setTimestamp(std::chrono::time_point<std::chrono::system_clock> timestamp);

    /// Name getter
    std::string getName() const;

    /// Timestamp getter
    std::chrono::time_point<std::chrono::system_clock> getTimestamp() const;

    /// Fields getter
    std::string getFields() const;

    /// Tags getter
    std::string getTags() const;

  protected:
    /// A value
    std::variant<long long int, std::string, double> mValue;

    /// A name
    std::string mMeasurement;

    /// A timestamp
    std::chrono::time_point<std::chrono::system_clock> mTimestamp;

    /// Tags
    std::string mTags;

    /// Fields
    std::string mFields;
};

} // namespace influxdb

#endif // INFLUXDATA_POINT_H
