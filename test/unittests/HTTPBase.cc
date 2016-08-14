#include "../../inc/http/HTTPBase.h"
#include <gtest/gtest.h>

using namespace collie::http;

TEST(HTTPBase, methodToString) {
  EXPECT_EQ(Method::GET, to_http_method("GET"));
  EXPECT_EQ(Method::GET, to_http_method("get"));
  EXPECT_EQ(Method::GET, to_http_method("Get"));
  EXPECT_EQ(Method::POST, to_http_method("POST"));
  EXPECT_EQ(Method::DELETE, to_http_method("DELETE"));
  EXPECT_EQ(Method::TRACE, to_http_method("TRACE"));
  EXPECT_EQ(Method::HEAD, to_http_method("HEAD"));
  EXPECT_EQ(Method::CONNECT, to_http_method("CONNECT"));
  EXPECT_EQ(Method::OPTION, to_http_method("OPTION"));
}
