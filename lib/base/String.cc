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
}
}
