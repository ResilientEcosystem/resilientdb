#include  <InfluxDBFactory.h>
#include <boost/program_options.hpp>
#include <iostream>

using namespace influxdb;

int main(int argc, char* argv[]) {

  int count = 1;

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("count", boost::program_options::value<int>(), "Points count")
    ("url", boost::program_options::value<std::string>()->required(), "URL to InfluxDB database")
    ("buffer", boost::program_options::value<int>(), "Buffer size");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  auto db = InfluxDBFactory::Get(vm["url"].as<std::string>());

  if (vm.count("count")) {
    count = std::floor(vm["count"].as<int>()/2) + 1;
  }

  if (vm.count("buffer")) {
    db->batchOf(vm["buffer"].as<int>());
  }

  for(int i = 0; i <= count; i++) {
    db->write(Point{"int"}.addField("value1", 10).addField("value2", "11").addTag("tag1", "machine"));
    db->write(Point{"double"}.addField("value1", 10.10).addField("value2", "11.11").addTag("tag1", "machine"));
  }
  std::cout << "Written " << 2*count-1 << " points";
  if (vm.count("buffer")) { std::cout << " through " << vm["buffer"].as<int>(); }
  std::cout << " to " << vm["url"].as<std::string>() << std::endl;
}
