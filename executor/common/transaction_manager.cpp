/*
 * Copyright (c) 2019-2022 ExpoLab, UC Davis
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "executor/common/transaction_manager.h"

#include <glog/logging.h>

namespace resdb {

TransactionManager::TransactionManager(bool is_out_of_order, bool need_response)
    : is_out_of_order_(is_out_of_order), need_response_(need_response) {}

bool TransactionManager::IsOutOfOrder() { return is_out_of_order_; }

bool TransactionManager::NeedResponse() { return need_response_; }

std::unique_ptr<std::string> TransactionManager::ExecuteData(
    const std::string& request) {
  return std::make_unique<std::string>();
}

std::unique_ptr<BatchUserResponse> TransactionManager::ExecuteBatch(
    const BatchUserRequest& request) {
  LOG(ERROR)<<"EXEC BATCH TEST";
  std::unique_ptr<BatchUserResponse> batch_response =
      std::make_unique<BatchUserResponse>();
  for (auto& sub_request : request.user_requests()) {
    std::unique_ptr<std::string> response =
        ExecuteData(sub_request.request().data());
    if (response == nullptr) {
      response = std::make_unique<std::string>();
    }
    batch_response->add_response()->swap(*response);
  }

  return batch_response;
}

}  // namespace resdb
