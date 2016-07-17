#ifndef COLLIE_BASE_STRING_H_
#define COLLIE_BASE_STRING_H_

#include <cstring>
#include <ostream>
#include "../collie.h"

namespace collie {

class String final {
 public:
  template <SizeType size>
  String(const char (&chars)[size]) noexcept : length(::strlen(chars)),
                                               capacity(length),
                                               data(new char[capacity]) {
    ::strcpy(data, chars);
  }
  String(const char* chars) noexcept : length(::strlen(chars)),
                                       capacity(length),
                                       data(new char[capacity]) {
    ::strcpy(data, chars);
  }
  constexpr String() noexcept : length(0), capacity(0), data(nullptr) {}

  // Copy
  String(const String& that) noexcept;
  String& operator=(const String& that) noexcept;
  // Move

  String(String&& that) noexcept;
  String& operator=(String&& that) noexcept;

  ~String() noexcept;

  String& TrimLeft() noexcept;
  String& TrimRight() noexcept;
  String& Trim() noexcept;
  String& Replace(const String& old_value, const String& new_value) noexcept;
  String& Remove(const String&) noexcept;
  String& Append(const String&) noexcept;
  SizeType Length() const noexcept { return length; }
  SizeType Capacity() const noexcept { return capacity; }
  const char* RawData() const noexcept { return data; }

  friend std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.data;
    return os;
  }

  String& operator+=(const String&) noexcept;
  String operator+(const String&) const noexcept;
  const char& operator[](const SizeType) const;
  char& operator[](const SizeType);
  bool operator==(const String&) const noexcept;
  bool operator!=(const String&) const noexcept;

  static String From(int) noexcept;
  static String From(long) noexcept;
  static String From(long long) noexcept;
  static String From(unsigned) noexcept;
  static String From(unsigned long) noexcept;
  static String From(unsigned long long) noexcept;
  // FIXME precise
  static String From(double) noexcept;
  static String From(long double) noexcept;
  static String From(float) noexcept;
  static String From(bool) noexcept;

 private:
  SizeType length;
  SizeType capacity;
  char* data;
};
}

#endif /* COLLIE_BASE_STRING_H_ */
