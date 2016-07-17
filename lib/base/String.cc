#include "../../inc/base/String.h"

namespace collie {
namespace base {

String::String(const String& that) noexcept {
  if (!that.data) {
    data = nullptr;
    length = 0;
  } else {
    data = new char[that.length];
    ::strcpy(data, that.data);
    length = that.length;
    capacity = length;
  }
}

String& String::operator=(const String& that) noexcept {
  if (!that.data) {
    delete[] data;
    data = nullptr;
    length = capacity = 0;
    return *this;
  }

  if (capacity >= that.length) {
    ::strncpy(data, that.data, that.length);
    length = that.length;
    if (capacity > length) data[length] = '\0';
  } else {
    if (data) {
      delete[] data;
    }
    data = new char[that.length];
    length = capacity = that.length;
    ::strcpy(data, that.data);
  }

  return *this;
}

String::String(String&& that) noexcept : length(that.length),
                                         capacity(that.capacity),
                                         data(that.data) {
  that.data = nullptr;
  that.length = that.capacity = 0;
}

String& String::operator=(String&& that) noexcept {
  if (data) delete[] data;
  length = that.length;
  capacity = that.capacity;
  data = that.data;

  that.length = that.capacity = 0;
  that.data = nullptr;
  return *this;
}

String::~String() noexcept {
  if (data) delete[] data;
}

String& String::TrimLeft() noexcept {
  if (length == 0) return *this;
  bool is_content_begin = false;
  SizeType new_length = length;
  for (SizeType x = 0, i = 0; i < length; ++i) {
    if (!is_content_begin) {
      if (data[i] == ' ') {
        continue;
      }
      is_content_begin = true;
      new_length = length - i;
    }
    data[x++] = data[i];
  }
  length = new_length;
  return *this;
}

String& String::TrimRight() noexcept {
  if (length == 0) return *this;
  SizeType new_length = length;
  for (int i = length - 1; i >= 0; --i) {
    if (data[i] == ' ') {
      --new_length;
    } else {
      break;
    }
  }
  data[new_length] = '\0';
  length = new_length;
  return *this;
}

String& String::Trim() noexcept {
  if (length == 0) return *this;
  TrimLeft();
  TrimRight();
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

String& String::Append(const String& that) noexcept {
  *this += that;
  return *this;
}

String& String::operator+=(const String& that) noexcept {
  if (!that.data || that.length == 0) return *this;

  if (that.length <= capacity - length) {
    for (SizeType i = length, j = 0; i < capacity; ++i, ++j) {
      data[i] = that.data[j];
    }
  } else if (!data) {
    capacity = that.length;
    data = new char[capacity];
    ::strncpy(data, that.data, that.length);
    length = capacity;
  } else {
    const auto new_capacity = length + that.length;
    auto new_data = new char[new_capacity];
    ::strncpy(new_data, data, length);
    for (SizeType i = length, j = 0; i < new_capacity; ++i, ++j) {
      new_data[i] = that.data[j];
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
    length = capacity;
  }

  return *this;
}

String String::operator+(const String& that) const noexcept {
  if (!data || length == 0) return that;
  if (!that.data || that.length == 0) return *this;

  String str;
  str.capacity = length + that.length;
  str.length = length + that.length;
  str.data = new char[str.capacity];

  ::strncpy(str.data, data, length);

  for (SizeType i = length, j = 0; i < str.length; ++i, ++j) {
    str.data[i] = that.data[j];
  }

  return str;
}

bool String::operator==(const String& that) const noexcept {
  if (length != that.length) return false;
  if (length == 0) return true;

  for (SizeType i = 0; i < length; ++i) {
    if (data[i] != that.data[i]) return false;
  }

  return true;
}

bool String::operator!=(const String& that) const noexcept {
  return !(*this == that);
}

const char& String::operator[](const SizeType index) const {
  if (index >= length)
    throw std::out_of_range("index of string is out of range");
  return data[index];
}
char& String::operator[](const SizeType index) {
  if (index >= length)
    throw std::out_of_range("index of string is out of range");
  return data[index];
}
}
}
