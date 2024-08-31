///
/// \author Adam Wegrzynek
///

#ifndef INFLUXDATA_INFLUXDB_FACTORY_H
#define INFLUXDATA_INFLUXDB_FACTORY_H

#include "InfluxDB.h"
#include "Transport.h"

namespace influxdb
{

/// \brief InfluxDB factory
class InfluxDBFactory
{
 public:
   /// Disables copy constructor
   InfluxDBFactory & operator=(const InfluxDBFactory&) = delete;
 
   /// Disables copy constructor 
   InfluxDBFactory(const InfluxDBFactory&) = delete;

   /// InfluxDB factory
   /// Provides InfluxDB instance with given transport
   /// \param url 	URL defining transport details
   /// \throw InfluxDBException 	if unrecognised backend or missing protocol
   static std::unique_ptr<InfluxDB> Get(std::string url) noexcept(false);
  
 private:
   ///\return  backend based on provided URL
   static std::unique_ptr<Transport> GetTransport(std::string url);

   /// Private constructor disallows to create instance of Factory
   InfluxDBFactory() = default;
};

} // namespace influxdb

#endif // INFLUXDATA_INFLUXDB_FACTORY_H
