///
/// \author Adam Wegrzynek
///

#ifndef INFLUXDATA_TRANSPORTINTERFACE_H
#define INFLUXDATA_TRANSPORTINTERFACE_H

#include <string>
#include <stdexcept>

namespace influxdb
{

/// \brief Transport interface
class Transport
{
  public:
    Transport() = default;

    virtual ~Transport() = default;

    /// Sends string blob
    virtual void send(std::string&& message) = 0;

    /// Sends s request
    virtual std::string query(const std::string& /*query*/) {
      throw std::runtime_error("Queries are not supported in the selected transport");
    }
};

} // namespace influxdb

#endif // INFLUXDATA_TRANSPORTINTERFACE_H
