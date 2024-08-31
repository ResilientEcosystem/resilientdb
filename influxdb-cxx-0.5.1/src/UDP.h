///
/// \author Adam Wegrzynek
///

#ifndef INFLUXDATA_TRANSPORTS_UDP_H
#define INFLUXDATA_TRANSPORTS_UDP_H

#include "Transport.h"

#include <boost/asio.hpp>
#include <chrono>
#include <string>

namespace influxdb
{
namespace transports
{

/// \brief UDP transport
class UDP : public Transport
{
  public:
    /// Constructor
    UDP(const std::string &hostname, int port);

    /// Default destructor
    ~UDP() = default;
 
    /// Sends blob via UDP
    void send(std::string&& message) override;   

  private:
    /// Boost Asio I/O functionality
    boost::asio::io_service mIoService;

    /// UDP socket
    boost::asio::ip::udp::socket mSocket;

    /// UDP endpoint
    boost::asio::ip::udp::endpoint mEndpoint;

};

} // namespace transports
} // namespace influxdb

#endif // INFLUXDATA_TRANSPORTS_UDP_H
