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
