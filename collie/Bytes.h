#pragma once

#include <collie/String.h>
#include <cstring>
#include <memory>

namespace collie {

class Bytes final {
 public:
  Bytes(const char* content, unsigned size)
      : content(new unsigned char[size], [](unsigned char* p) { delete[] p; }),
        size(size) {
    ::strncpy(reinterpret_cast<char*>(this->content.get()), content, size);
  }

  Bytes(const unsigned char* content, unsigned size)
      : Bytes(reinterpret_cast<const char*>(content), size) {}

  explicit Bytes(const std::string& str) : Bytes(str.c_str(), str.size()) {}

  Bytes(const Bytes& that) : content(that.content), size(that.size) {}
  Bytes& operator=(const Bytes& that) {
    content = that.content;
    size = that.size;
    return *this;
  }
  ~Bytes() {}

  const unsigned char* Content() const noexcept { return content.get(); }
  unsigned Size() const noexcept { return size; }

  std::string ToString() {
    return std::string(reinterpret_cast<const char*>(content.get()));
  }

 private:
  std::shared_ptr<unsigned char> content;
  unsigned size;
};
}
