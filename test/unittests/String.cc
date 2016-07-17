#include "../../inc/base/String.h"
#include <gtest/gtest.h>

using namespace collie::base;

TEST(String, DefaultConstructor) {
  String s1;
  ASSERT_EQ(0, s1.Length());
  ASSERT_GE(s1.Capacity(), s1.Length());
  ASSERT_EQ(nullptr, s1.RawData());

  String s2("abcd");
  ASSERT_EQ(4, s2.Length());
  ASSERT_GE(s2.Capacity(), s2.Length());
  ASSERT_STREQ("abcd", s2.RawData());

  String s3("");
  ASSERT_EQ(0, s3.Length());
  ASSERT_GE(s3.Capacity(), s3.Length());
  ASSERT_STREQ("", s3.RawData());

  const char* c1 = "abcdef";
  String s4(c1);
  ASSERT_EQ(6, s4.Length());
  ASSERT_GE(s4.Capacity(), s4.Length());
  ASSERT_STREQ("abcdef", s4.RawData());

  char c2[] = "abcdef";
  String s5(c2);
  ASSERT_EQ(6, s5.Length());
  ASSERT_GE(s5.Capacity(), s5.Length());
  ASSERT_STREQ("abcdef", s5.RawData());

  char c3[] = "abcdef";
  String s6(c3);
  ASSERT_EQ(6, s6.Length());
  ASSERT_GE(s6.Capacity(), s6.Length());
  ASSERT_STREQ("abcdef", s6.RawData());
}

TEST(String, CopyConstructor) {
  String s1("abcdef");
  String s2(s1);
  ASSERT_STREQ("abcdef", s2.RawData());
  ASSERT_EQ(s1.Length(), s2.Length());
  ASSERT_GE(s2.Capacity(), s2.Length());

  String s3;
  String s4(s3);
  ASSERT_EQ(nullptr, s4.RawData());
  ASSERT_EQ(s3.Length(), s4.Length());
  ASSERT_GE(s4.Capacity(), s4.Length());

  String s5("abcdef");
  String s6;
  s6 = s5;
  ASSERT_STREQ("abcdef", s6.RawData());
  ASSERT_EQ(s6.Length(), s5.Length());
  ASSERT_GE(s6.Capacity(), s6.Length());
}

TEST(String, MoveConstructor) {
  String s1("abcdef");
  String s2(std::move(s1));
  ASSERT_STREQ("abcdef", s2.RawData());
  ASSERT_EQ(6, s2.Length());
  ASSERT_GE(s2.Capacity(), s2.Length());
  ASSERT_EQ(0, s1.Length());
  ASSERT_EQ(0, s1.Capacity());

  String s3;
  String s4(std::move(s3));
  ASSERT_EQ(nullptr, s4.RawData());
  ASSERT_EQ(s3.Length(), s4.Length());
  ASSERT_GE(s4.Capacity(), s4.Length());
}

TEST(String, Trim) {
  String s1("   ab  cd ef   g    ");
  s1.Trim();
  ASSERT_EQ(13, s1.Length());
  ASSERT_STREQ("ab  cd ef   g", s1.RawData());
}

TEST(String, FromPrimitives) {
  ASSERT_TRUE(String::From(true) == "true");
  ASSERT_TRUE(String::From(false) == "false");
  int n1 = 123;
  ASSERT_TRUE(String::From(n1) == "123");
  long n2 = 123456789;
  ASSERT_TRUE(String::From(n2) == "123456789");
  long long n3 = -1234567890123456;
  ASSERT_TRUE(String::From(n3) == "-1234567890123456");
  unsigned n4 = 12345;
  ASSERT_TRUE(String::From(n4) == "12345");
  unsigned long long n5 = 1234567890123456;
  ASSERT_TRUE(String::From(n5) == "1234567890123456");
  double n6 = 123.456789;
  ASSERT_TRUE(String::From(n6) == "123.456789");
  float n7 = 123.45678;
  ASSERT_TRUE(String::From(n7) == "123.456779");
  long double n8 = 123.456789123;
  ASSERT_TRUE(String::From(n8) == "123.456789");
}

TEST(String, OperatorEqual) {
  String s1;
  String s2;
  String s3("");
  ASSERT_TRUE(s1 == s2);
  ASSERT_TRUE(s1 == s3);
  ASSERT_TRUE(s3 == s2);
  ASSERT_FALSE(s1 != s2);
  ASSERT_FALSE(s1 != s3);
  ASSERT_FALSE(s3 != s2);

  String s4("abcde");
  String s5("abcde");
  String s6("abcdef");
  ASSERT_TRUE(s4 == s5);
  ASSERT_TRUE(s5 == s4);
  ASSERT_FALSE(s4 != s5);
  ASSERT_FALSE(s5 != s4);
  ASSERT_TRUE(s4 != s6);
  ASSERT_TRUE(s6 != s4);
  ASSERT_FALSE(s4 == s6);
  ASSERT_FALSE(s6 == s4);
}

TEST(String, OperatorAppend) {
  String s1;
  String s2("");
  ASSERT_TRUE(s1 + s1 == "");
  ASSERT_TRUE(s1 + s2 == "");
  ASSERT_TRUE(s2 + s1 == "");
  ASSERT_TRUE(s2 + s2 == "");
  s1 += s1;
  ASSERT_TRUE(s1 == "");

  String s3("abcde");
  String s4("fghij");
  String s5 = s3 + s4;
  ASSERT_TRUE(s5 == "abcdefghij");
  ASSERT_EQ(10, s5.Length());
  ASSERT_GE(s5.Capacity(), s5.Length());
  s4 = "f";
  s5 = s3 + s4;
  ASSERT_TRUE(s5 == "abcdef");
  ASSERT_EQ(6, s5.Length());
  ASSERT_GE(s5.Capacity(), s5.Length());

  s5 += "ghijklmn";
  ASSERT_TRUE(s5 == "abcdefghijklmn");
  ASSERT_EQ(14, s5.Length());
  ASSERT_GE(s5.Capacity(), s5.Length());
}

TEST(String, OperatorIndex) {
  String s1("abcdefg");
  ASSERT_THROW(s1[7], std::out_of_range);
  ASSERT_NO_THROW(s1[0]);
  ASSERT_NO_THROW(s1[6]);
  s1[5] = 'g';
  ASSERT_TRUE(s1 == "abcdegg");
  s1[3] = 'u';
  ASSERT_TRUE(s1 == "abcuegg");
}
