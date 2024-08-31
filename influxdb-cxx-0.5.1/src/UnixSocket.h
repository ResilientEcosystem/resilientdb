///
/// \author Adam Wegrzynek
///

#ifndef INFLUXDATA_TRANSPORTS_UNIX_H
#define INFLUXDATA_TRANSPORTS_UNIX_H

#include "Transport.h"

#include <boost/asio.hpp>
#include <string>

namespace influxdb
{
namespace transports
{

/// \brief Unix datagram socket transport
class UnixSocket : public Transport
{
  public:
    UnixSocket(const std::string &socketPath);

    /// Default destructor
    ~UnixSocket() = default;
 
    /// \param message   r-value string formated
    void send(std::string&& message) override;   

  private:
    /// Boost Asio I/O functionality
    boost::asio::io_service mIoService;
#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
    /// Unix socket
    boost::asio::local::datagram_protocol::socket mSocket;

    /// Unix endpoint
    boost::asio::local::datagram_protocol::endpoint mEndpoint;
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
};

} // namespace transports
} // namespace influxdb

#endif // INFLUXDATA_TRANSPORTS_UNIX_H
