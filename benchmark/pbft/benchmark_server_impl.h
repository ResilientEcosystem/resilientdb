#pragma once

#include <chrono>
#include <future>
#include <map>

#include "execution/transaction_executor_impl.h"

namespace resdb {

class BenchmarkServerImpl : public TransactionExecutorImpl {
 public:
  BenchmarkServerImpl();
  virtual ~BenchmarkServerImpl() = default;

  std::unique_ptr<std::string> ExecuteData(const std::string& request) override;

 private:
  std::thread thread_;
  std::promise<bool> begin_;
};

}  // namespace resdb
