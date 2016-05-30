#ifndef COLLIE_UTIL_STRING_H_
#define COLLIE_UTIL_STRING_H_

#include <string>
#include "../collie.h"
#include "iterable.h"

namespace collie {
namespace util {

constexpr Size length_of_char(const char* str) {
  return *str ? 1 + length_of_char(str + 1) : 0;
}

class StringIterator : public Iterator<char> {
 public:
  using Self = StringIterator;
  constexpr StringIterator(const char* value, Size position) noexcept
      : value(value),
        position(position) {}
  ~StringIterator() noexcept override {}

  Self& operator++() noexcept override { return *this; }
  Self& operator+(unsigned) noexcept override { return *this; }

  Self& operator--() noexcept override { return *this; }
  Self& operator-(unsigned) noexcept override { return *this; }

  bool operator==(const Iterator<char>&) const noexcept override {
    return true;
  }
  bool operator!=(const Iterator<char>&) const noexcept override {
    return true;
  }

  const char& operator*() const noexcept override { return *value; }

  char operator->() const noexcept override { return *value; }

 protected:
  const char* value;
  Size position;
};

class String : public NonCopyable, public Iterable<char> {
 public:
  using Iterator = StringIterator;

  constexpr String() noexcept : buffer(nullptr), length(0) {}

  template <Size N>
  constexpr String(const char (&buffer)[N]) noexcept : buffer(buffer),
                                                       length(N) {}
  constexpr String(const char* buffer) noexcept
      : buffer(buffer),
        length(length_of_char(buffer)) {}

  String(const std::string& str) noexcept : buffer(str.c_str()),
                                            length(str.length()) {}

  ~String() noexcept override {}

  String* operator+(const String*) noexcept;
  String* Replace(const String* old_value, const String* new_value) noexcept;
  String* Trim() noexcept;
  Size Length() const { return length; }

  Iterator* begin() noexcept override { return Begin(); }
  Iterator* end() noexcept override { return End(); }
  const Iterator* begin() const noexcept override { return Begin(); }
  const Iterator* end() const noexcept override { return End(); }

  Iterator* Begin() noexcept override { return new Iterator(buffer, 0); }
  Iterator* End() noexcept override { return new Iterator(buffer, 0); }
  const Iterator* Begin() const noexcept override {
    return new Iterator(buffer, length);
  }
  const Iterator* End() const noexcept override {
    return new Iterator(buffer, length);
  }

 private:
  const char* buffer;
  const Size length;
};
}
}

#endif /* COLLIE_UTIL_STRING_H_ */
