#include "kv_server/kv_server_executor.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "proto/kv_server.pb.h"

namespace resdb {
namespace {

using ::testing::Test;

class KVServerExecutorTest : public Test {
 public:
  int Set(const std::string& key, const std::string& value) {
    KVRequest request;
    request.set_cmd(KVRequest::SET);
    request.set_key(key);
    request.set_value(value);

    std::string str;
    if (!request.SerializeToString(&str)) {
      return -1;
    }
    impl_.ExecuteData(str);
    return 0;
  }

  std::string Get(const std::string& key) {
    KVRequest request;
    request.set_cmd(KVRequest::GET);
    request.set_key(key);

    std::string str;
    if (!request.SerializeToString(&str)) {
      return "";
    }
    auto resp = impl_.ExecuteData(str);
    if (resp == nullptr) {
      return "";
    }
    KVResponse kv_response;
    if (!kv_response.ParseFromString(*resp)) {
      return "";
    }
    return kv_response.value();
  }

 private:
  KVServerExecutor impl_;
};

TEST_F(KVServerExecutorTest, SetValue) {
  EXPECT_EQ(Set("test_key", "test_value"), 0);
  EXPECT_EQ(Get("test_key"), "test_value");
}

TEST_F(KVServerExecutorTest, GetValue) { EXPECT_EQ(Get("test_key"), ""); }

}  // namespace

}  // namespace resdb
