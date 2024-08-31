#pragma once

#include "execution/transaction_executor.h"
#include "gmock/gmock.h"

namespace resdb {

class MockTransactionExecutorImpl : public TransactionExecutorImpl {
 public:
  MOCK_METHOD(std::unique_ptr<std::string>, ExecuteData, (const std::string&),
              (override));
};

}  // namespace resdb
