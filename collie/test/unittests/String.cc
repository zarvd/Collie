#include "../../inc/base/String.h"
#include <gtest/gtest.h>

using namespace collie::string;

TEST(String, split) {
  const std::string s1("123,456,789");
  const auto v1 = split(s1, ",");
  ASSERT_EQ(3, v1.size());
  ASSERT_EQ("123", v1[0]);
  ASSERT_EQ("456", v1[1]);
  ASSERT_EQ("789", v1[2]);

  const std::string s2("123, 456, 789,012,  345");
  const auto v2 = split(s2, ", ");
  ASSERT_EQ(4, v2.size());
  ASSERT_EQ("123", v2[0]);
  ASSERT_EQ("456", v2[1]);
  ASSERT_EQ("789,012", v2[2]);
  ASSERT_EQ(" 345", v2[3]);

  const std::string s3 = std::string("POST /blah HTTP/1.1\r\n") +
                         "Host: example.com\r\n" +
                         "Accept-Language: en-us,en;q=0.5\r\n" +
                         "Accept-Encoding: gzip,deflate\r\n" +
                         "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n" +
                         "Content-Type: application/x-www-form-urlencoded\r\n" +
                         "Content-Length: 25\r\n\r\n";
  const auto v3 = split(s3, "\r\n");
  ASSERT_EQ(7, v3.size());
  ASSERT_EQ("POST /blah HTTP/1.1", v3[0]);
  ASSERT_EQ("Host: example.com", v3[1]);
  ASSERT_EQ("Accept-Language: en-us,en;q=0.5", v3[2]);
  ASSERT_EQ("Accept-Encoding: gzip,deflate", v3[3]);
  ASSERT_EQ("Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7", v3[4]);
  ASSERT_EQ("Content-Type: application/x-www-form-urlencoded", v3[5]);
  ASSERT_EQ("Content-Length: 25", v3[6]);
}
