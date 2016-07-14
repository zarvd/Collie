#ifndef COLLIE_BASE_STRING_H_
#define COLLIE_BASE_STRING_H_

#include <string.h>
#include "../collie.h"

namespace collie {
namespace base {

class String {
 public:
  template <Size size>
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

  String& Trim() noexcept;
  String& Replace(const String& old_value, const String& new_value) noexcept;
  String& Remove(const String&) noexcept;
  Size Length() const noexcept { return length; }
  Size Capacity() const noexcept { return capacity; }
  const char* RawData() const noexcept { return data; }

 private:
  Size length;
  Size capacity;
  char* data;
};
}
}

#endif /* COLLIE_BASE_STRING_H_ */