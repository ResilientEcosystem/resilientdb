///
/// \author Adam Wegrzynek
///

#ifndef INFLUXDATA_TRANSPORTS_HTTP_H
#define INFLUXDATA_TRANSPORTS_HTTP_H

#include "Transport.h"
#include <curl/curl.h>
#include <memory>
#include <string>

namespace influxdb
{
namespace transports
{

/// \brief HTTP transport
class HTTP : public Transport
{
  public:
    /// Constructor
    HTTP(const std::string& url);

    /// Default destructor
    ~HTTP();

    /// Sends point via HTTP POST
    ///  \throw InfluxDBException	when CURL fails on POSTing or response code != 200
    void send(std::string&& post) override;

    /// Queries database
    /// \throw InfluxDBException	when CURL GET fails
    std::string query(const std::string& query) override;

    /// Enable Basic Auth
    /// \param auth <username>:<password>
    void enableBasicAuth(const std::string& auth);

    /// Enable SSL
    void enableSsl();
  private:

    /// Initilizes CURL for writting and common options
    /// \throw InfluxDBException	if database (?db=) not specified
    void initCurl(const std::string& url);

    /// Initializes CURL for reading
    void initCurlRead(const std::string& url);

    /// CURL pointer configured for writting points
    CURL* writeHandle;

    /// CURL poiter confgured for querying
    CURL* readHandle;

    /// InfluxDB read URL
    std::string mReadUrl;
};

} // namespace transports
} // namespace influxdb

#endif // INFLUXDATA_TRANSPORTS_HTTP_H
