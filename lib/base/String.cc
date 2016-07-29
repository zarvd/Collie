#include "../../inc/base/String.h"

namespace collie {

String::String(const String& that) noexcept : data(that.data) {}

String& String::operator=(const String& that) noexcept {
  data = that.data;

  return *this;
}

String::String(String&& that) noexcept : data(std::move(that.data)) {}

String& String::operator=(String&& that) noexcept {
  data = std::move(that.data);
  return *this;
}

String::~String() noexcept {}

String& String::TrimLeft(const String& str) noexcept {
  if (IsNull() || str.IsNull() || Length() < str.Length()) return *this;
  unsigned i = 0, j = 0;
  while (i < Length()) {
    if (data[i] != str.data[j]) break;
    ++i;
    ++j;
    if (j == str.Length()) j = 0;
  }
  data = data.substr(i);
  return *this;
}

String& String::TrimRight(const String& str) noexcept {
  if (IsNull() || str.IsNull() || Length() < str.Length()) return *this;
  int i = Length() - 1, j = str.Length() - 1;
  while (i >= 0) {
    if (data[i] != str.data[j]) break;
    --i;
    --j;

    if (j < 0) j = str.Length() - 1;
  }
  data = data.substr(0, i + 1);
  return *this;
}

String& String::Trim(const String& str) noexcept {
  if (IsNull() || str.IsNull() || Length() < str.Length()) return *this;
  TrimLeft(str);
  TrimRight(str);
  return *this;
}

String String::From(int num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%d", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(long num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%ld", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(long long num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%lld", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(float num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%f", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(double num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%f", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(long double num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%Lf", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(unsigned num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%u", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(unsigned long num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%lu", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(unsigned long long num) noexcept {
  char* str = new char[16];
  ::sprintf(str, "%llu", num);
  String s(str);
  delete[] str;
  return s;
}

String String::From(bool cond) noexcept {
  return String(cond ? "true" : "false");
}

String String::From(const char* str) noexcept { return str; }

String& String::Append(const String& that) noexcept {
  *this += that;
  return *this;
}

String String::Slice(const SizeType index, const SizeType length) const
    noexcept {
  if (length == 0) return data.substr(index);
  return data.substr(index, length);
}

// KMP Algorithm
bool String::Contain(const String& that) const noexcept {
  return data.find(that.data) != std::string::npos;
  // if (that.IsNull()) return true;
  // if (that.Length() > Length()) return false;
  // int next[that.Length()];
  // next[0] = -1;
  // for (SizeType i = 1; i < that.Length(); ++i) {
  //   int k = next[i - 1];
  //   while (k >= 0) {
  //     if (that.data[k] == that.data[i - 1]) break;
  //     k = next[k];
  //   }
  //   next[i] = k + 1;
  // }

  // SizeType i = 0;
  // int k = 0;
  // while (i < Length()) {
  //   if (data[i] == that.data[k]) {
  //     ++i;
  //     ++k;
  //     if (that.Length() == (SizeType)k) return true;
  //   } else {
  //     k = next[k];
  //     if (k == -1) {
  //       ++i;
  //       k = 0;
  //     }
  //   }
  // }
  // return false;
}

String& String::operator+=(const String& that) noexcept {
  data += that.data;

  return *this;
}

String String::operator+(const String& that) const noexcept {
  return data + that.data;
}

bool String::operator==(const String& that) const noexcept {
  return data == that.data;
}

bool String::operator!=(const String& that) const noexcept {
  return !(*this == that);
}

const char& String::operator[](const SizeType index) const {
  if (index >= Length())
    throw std::out_of_range("index of string is out of range");
  return data[index];
}
char& String::operator[](const SizeType index) {
  if (index >= Length())
    throw std::out_of_range("index of string is out of range");
  return data[index];
}
}
