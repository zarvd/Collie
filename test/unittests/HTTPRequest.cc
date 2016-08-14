#include "../../inc/http/HTTPRequest.h"
#include <gtest/gtest.h>

using namespace collie::http;

TEST(HTTPRequest, parse) {
  const auto req_str = std::string("GET /blah?name=currant HTTP/1.1\r\n") +
                       "Host: example.com\r\n" +
                       "Accept-Language: en-us,en;q=0.5\r\n" +
                       "Accept-Encoding: gzip,deflate\r\n" +
                       "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n" +
                       "Content-Type: application/x-www-form-urlencoded\r\n" +
                       "Content-Length: 25\r\n\r\n";

  HTTPRequest req(req_str);

  EXPECT_EQ(Method::GET, req.Method()) << to_string(req.Method());
  EXPECT_EQ(std::string("/blah"), req.Url());
  EXPECT_TRUE(req.HasQuery("name"));
  const auto queries = req.QueryArray("name");
  EXPECT_EQ(1, queries.size());
  EXPECT_EQ(std::string("currant"), queries[0]);
}
