#ifndef COLLIE_BASE_STRING_H_
#define COLLIE_BASE_STRING_H_

#include <ostream>
#include <string>
#include "../collie.h"

namespace collie {

class String final {
 public:
  String(const char* str) noexcept : data(str) {}
  String(const std::string& str = "") noexcept : data(str) {}

  // Copy
  String(const String& that) noexcept;
  String& operator=(const String& that) noexcept;

  // Move
  String(String&& that) noexcept;
  String& operator=(String&& that) noexcept;

  ~String() noexcept;

  String& TrimLeft(const String& = " ") noexcept;
  String& TrimRight(const String& = " ") noexcept;
  String& Trim(const String& = " ") noexcept;
  String& Replace(const String& old_value, const String& new_value) noexcept;
  String& Remove(const String&) noexcept;
  String& Append(const String&) noexcept;
  String Slice(const SizeType index, const SizeType length = 0) const noexcept;
  SizeType Length() const noexcept { return data.length(); }
  SizeType Capacity() const noexcept { return data.capacity(); }
  bool IsNull() const noexcept { return data.empty(); }
  bool Contain(const String&) const noexcept;
  const char* RawData() const noexcept { return data.c_str(); }

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
  static String From(const char*) noexcept;

 private:
  std::string data;
};
}

namespace std {

template <>
struct hash<collie::String> : public unary_function<collie::String, size_t> {
  size_t operator()(const collie::String& value) const {
    unsigned long h = 5381;
    const char* str = value.RawData();
    int c = *str++;
    while (c) {
      h = ((h << 5) + h) + c; /* h * 33 + c */
      c = *str++;
    }
    return h;
  }
};
}

#endif /* COLLIE_BASE_STRING_H_ */
