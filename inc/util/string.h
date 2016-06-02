#ifndef COLLIE_UTIL_STRING_H_
#define COLLIE_UTIL_STRING_H_

#include <string>
#include "../collie.h"
#include "iterator.h"
#include "noncopyable.h"

namespace collie {
namespace util {

constexpr Size length_of_char(const char* str) {
  return *str ? 1 + length_of_char(str + 1) : 0;
}

class StringIterator : public ConstIterator<StringIterator, char> {
 public:
  using Self = StringIterator;
  constexpr StringIterator(const char* value, Size length,
                           Size position) noexcept : value(value),
                                                     position(position),
                                                     length(length) {}

  constexpr StringIterator(const Self& that) noexcept : value(that.value),
                                                        position(that.position),
                                                        length(that.length) {}

  ~StringIterator() noexcept {}

  virtual bool HasNext() const noexcept override {
    return position + 1 < length;
  }
  virtual bool HasPrevious() const noexcept override { return position > 0; }

  virtual const char& GetValue() const noexcept override {
    return value[position];
  }

  virtual void ToNext(const unsigned offset = 1) noexcept override {
    position += offset;
  }
  virtual void ToPrevious(const unsigned offset = 1) noexcept override {
    position -= offset;
  }

  virtual void ToFirst() noexcept override { position = 0; }
  virtual void ToLast() noexcept override { position = length - 1; }

 protected:
  const char* const value;
  Size position;
  Size length;
};

class String : public NonCopyable, public ConstIterable<StringIterator, char> {
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

  String operator+(const String*) const noexcept;
  String Replace(const String* old_value, const String* new_value) const
      noexcept;
  String Trim() const noexcept;
  Size Length() const noexcept { return length; }

  Iterator GetIterator() const noexcept override {
    return Iterator(buffer, 0, length);
  }
  Iterator GetReversedIterator() const noexcept override {
    return Iterator(buffer, length - 1, length);
  }

 private:
  const char* const buffer;
  const Size length;
};
}
}

#endif /* COLLIE_UTIL_STRING_H_ */
