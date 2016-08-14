#ifndef COLLIE_HTTP_HTTP_HEADER_H_
#define COLLIE_HTTP_HTTP_HEADER_H_

#include <initializer_list>
#include <unordered_map>
#include <utility>
#include "../../inc/base/Serializable.h"
#include "../../inc/base/String.h"

namespace collie {
namespace http {

class HTTPHeader final : public Serializable {
 public:
  explicit HTTPHeader(const std::unordered_map<String, String>& headers)
      : headers(headers) {}
  explicit HTTPHeader(const String& header_buffer);
  ~HTTPHeader() override;

  // HTTPHeader(const HTTPHeader&) noexcept;
  // HTTPHeader& operator=(const HTTPHeader&) noexcept;

  // HTTPHeader(HTTPHeader&&) noexcept;
  // HTTPHeader& operator=(HTTPHeader&&) noexcept;

  String Get(const String& field) const noexcept {
    auto it = headers.find(field);
    if (it == headers.end()) return String();
    return it->second;
  }
  void Set(const String& field, const String& value) noexcept {
    headers.insert({field, value});
  }
  void Set(std::initializer_list<std::pair<String, String>>& headers) noexcept {
    for (const auto& header : headers) {
      this->headers.insert(header);
    }
  }

  String ToString() const noexcept override;

 private:
  std::unordered_map<String, String> headers;
};
}
}

#endif /* COLLIE_HTTP_HTTP_HEADER_H_ */
