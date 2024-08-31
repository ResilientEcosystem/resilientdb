#pragma once

#include <glog/logging.h>
#include <prometheus/counter.h>
#include <prometheus/histogram.h>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>

namespace resdb {

enum TableName {
  MINING,
  TRANSACTION,
  PERFORMANCE,
  MSG,
};

enum MetricName {
  MINING_TIME,
  TRANSACTION_INPUT,
  MINING_LATENCY,
  THROUGHPUT,
  TRANSACTION_LATENCY,
  SHIFT_MSG,
};

class PrometheusHandler {
 public:
  PrometheusHandler(const std::string& server_address);
  ~PrometheusHandler();

  void Set(MetricName name, double value);
  void SetValue(MetricName name, double value);
  void Inc(MetricName name, int num);
  void Inc(MetricName name, double value);

 protected:
  void Register();
  void RegisterGaugeMetric(const std::string& table_name,
                      const std::string& metric_name);
  void RegisterHistogramMetric(const std::string& table_name,
                      const std::string& metric_name);
  void RegisterCounterMetric(const std::string& table_name,
                      const std::string& metric_name);

  void RegisterGauge(const std::string& name);
  void RegisterHistogram(const std::string& name);
  void RegisterCounter(const std::string& name);

 private:
  typedef prometheus::Family<prometheus::Gauge> GaugeBuilder;
  typedef prometheus::Family<prometheus::Histogram> HistogramBuilder;
  typedef prometheus::Family<prometheus::Counter> CounterBuilder;

  std::unique_ptr<prometheus::Exposer, std::default_delete<prometheus::Exposer>>
      exposer_;
  std::shared_ptr<prometheus::Registry> registry_;

  std::map<std::string, GaugeBuilder*> gauge_builder_;
  std::map<std::string, HistogramBuilder*> histogram_builder_;
  std::map<std::string, CounterBuilder*> counter_builder_;
  std::map<std::string, prometheus::Histogram*> histogram_metric_;
  std::map<std::string, prometheus::Counter*> counter_metric_;
  std::map<std::string, prometheus::Gauge*> gauge_metric_;
};

}  // namespace resdb
