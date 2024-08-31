#pragma once

#include <map>

#include "execution/transaction_executor_impl.h"

namespace resdb {

class KVServerExecutor : public TransactionExecutorImpl {
 public:
  virtual ~KVServerExecutor() = default;

  std::unique_ptr<std::string> ExecuteData(const std::string& request) override;

 private:
  void Set(const std::string& key, const std::string& value);
  std::string Get(const std::string& key);

 private:
  std::map<std::string, std::string> kv_map_;
};

}  // namespace resdb
