#pragma once

#include <memory>

namespace resdb {

class TransactionExecutorImpl {
 public:
  TransactionExecutorImpl() = default;
  virtual ~TransactionExecutorImpl() = default;

  virtual std::unique_ptr<std::string> ExecuteData(const std::string& request);
};
}  // namespace resdb
