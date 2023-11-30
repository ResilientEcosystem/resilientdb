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

#pragma once

#include <map>
#include <optional>
#include <unordered_map>

#include "chain/storage/storage.h"
#include "executor/common/transaction_manager.h"
#include "proto/kv/kv.pb.h"

namespace resdb {

class KVExecutor : public TransactionManager {
 public:
  KVExecutor(std::unique_ptr<Storage> storage);
  virtual ~KVExecutor() = default;

  std::unique_ptr<std::string> ExecuteData(const std::string& request) override;

  Storage* GetStorage() override;

 protected:
  virtual void Set(const std::string& key, const std::string& value);
  std::string Get(const std::string& key);
  std::string GetAllValues();
  std::string GetRange(const std::string& min_key, const std::string& max_key);

  void SetWithVersion(const std::string& key, const std::string& value,
                      int version);
  void GetWithVersion(const std::string& key, int version, ValueInfo* info);
  void GetAllItems(Items* items);
  void GetKeyRange(const std::string& min_key, const std::string& max_key,
                   Items* items);
  void GetHistory(const std::string& key, int min_key, int max_key,
                  Items* items);
  void GetTopHistory(const std::string& key, int top_number, Items* items);

 private:
  std::unique_ptr<Storage> storage_;
};

}  // namespace resdb
