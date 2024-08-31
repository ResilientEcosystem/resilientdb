#include "statistic/prometheus_handler.h"

#include <glog/logging.h>

namespace resdb {

using prometheus::Histogram;

std::map<TableName, std::string> histogram_table_names = {
    {MINING, "mining"},
};

std::map<TableName, std::string> counter_table_names = {
    {TRANSACTION, "transaction"},
    {MSG, "msg"},
};

std::map<TableName, std::string> gauge_table_names = {
    {PERFORMANCE, "performance"},
};

std::map<MetricName, std::pair<TableName, std::string>> metric_names = {
    {MINING_TIME, {PERFORMANCE, "mining_time"}},
    {TRANSACTION_INPUT, {TRANSACTION, "tx_input"}},
    {MINING_LATENCY, {TRANSACTION, "mining_latency"}},
    {THROUGHPUT, {PERFORMANCE, "throughtput"}},
    {TRANSACTION_LATENCY, {PERFORMANCE, "latency"}},
    {SHIFT_MSG, {MSG, "shift_msg"}},
};

PrometheusHandler::PrometheusHandler(const std::string& server_address) {
  exposer_ =
      prometheus::detail::make_unique<prometheus::Exposer>(server_address);
	LOG(ERROR)<<"handler address:"<<server_address;
  registry_ = std::make_shared<prometheus::Registry>();
  exposer_->RegisterCollectable(registry_);

  Register();
}

PrometheusHandler::~PrometheusHandler() {
  if (exposer_) {
    exposer_->RemoveCollectable(registry_);
  }
}

void PrometheusHandler::Register() {
  for (auto& table : histogram_table_names) {
    RegisterHistogram(table.second);
  }

  for (auto& metric_pair : metric_names) {
	  if (histogram_table_names.find(metric_pair.second.first) == histogram_table_names.end()){
		  continue;
	  }
    RegisterHistogramMetric(histogram_table_names[metric_pair.second.first],
                   metric_pair.second.second);
  }
  
  for (auto& table : counter_table_names) {
    RegisterCounter(table.second);
  }

  for (auto& metric_pair : metric_names) {
	  if (counter_table_names.find(metric_pair.second.first) == counter_table_names.end()){
		  continue;
	  }
    RegisterCounterMetric(counter_table_names[metric_pair.second.first],
                   metric_pair.second.second);
  }
  
  
  for (auto& table : gauge_table_names) {
    RegisterGauge(table.second);
  }

  for (auto& metric_pair : metric_names) {
	  if (gauge_table_names.find(metric_pair.second.first) == gauge_table_names.end()){
		  continue;
	  }
    RegisterGaugeMetric(gauge_table_names[metric_pair.second.first],
                   metric_pair.second.second);
  }
}

void PrometheusHandler::RegisterHistogram(const std::string& name) {
  histogram_builder_[name] = &prometheus::BuildHistogram()
                         .Name(name)
                         .Help(name + " metrics")
                         .Register(*registry_);
}

void PrometheusHandler::RegisterCounter(const std::string& name) {
  counter_builder_[name] = &prometheus::BuildCounter()
                         .Name(name)
                         .Help(name + " metrics")
                         .Register(*registry_);
}

void PrometheusHandler::RegisterGauge(const std::string& name) {
  gauge_builder_[name] = &prometheus::BuildGauge()
                         .Name(name)
                         .Help(name + " metrics")
                         .Register(*registry_);
}

void PrometheusHandler::RegisterHistogramMetric(const std::string& table_name,
                                       const std::string& metric_name) {
  if (histogram_builder_.find(table_name) == histogram_builder_.end()) {
    return;
  }
  histogram_metric_[metric_name] = &histogram_builder_[table_name]->Add({{"metrics", metric_name}},
                                Histogram::BucketBoundaries{0,1,5});
}

void PrometheusHandler::RegisterCounterMetric(const std::string& table_name,
                                       const std::string& metric_name) {
  if (counter_builder_.find(table_name) == counter_builder_.end()) {
    return;
  }
  counter_metric_[metric_name] = &counter_builder_[table_name]->Add({{"metrics", metric_name}});
}

void PrometheusHandler::RegisterGaugeMetric(const std::string& table_name,
                                       const std::string& metric_name) {
  if (gauge_builder_.find(table_name) == gauge_builder_.end()) {
    return;
  }
  gauge_metric_[metric_name] = &gauge_builder_[table_name]->Add({{"metrics", metric_name}});
}

void PrometheusHandler::SetValue(MetricName name, double value) {
  std::string metric_name_str = metric_names[name].second;
  if (gauge_metric_.find(metric_name_str) == gauge_metric_.end()) {
    return;
  }
  gauge_metric_[metric_name_str]->Set(value);
  LOG(ERROR)<<"set value:"<<name<<" "<<value;
}

void PrometheusHandler::Set(MetricName name, double value) {
  std::string metric_name_str = metric_names[name].second;
  if (histogram_metric_.find(metric_name_str) == histogram_metric_.end()) {
    return;
  }
  histogram_metric_[metric_name_str]->Observe(value);
}

void PrometheusHandler::Inc(MetricName name, double value) {
  std::string metric_name_str = metric_names[name].second;
  if (counter_metric_.find(metric_name_str) == counter_metric_.end()) {
    return;
  }
  counter_metric_[metric_name_str]->Increment(value);
}

void PrometheusHandler::Inc(MetricName name, int num) {
  std::string metric_name_str = metric_names[name].second;
  if (counter_metric_.find(metric_name_str) == counter_metric_.end()) {
    return;
  }
  counter_metric_[metric_name_str]->Increment(num);
}

}  // namespace resdb
