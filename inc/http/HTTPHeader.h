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
  explicit HTTPHeader(
      const std::unordered_map<std::string, std::string>& headers)
      : headers(headers) {}
  explicit HTTPHeader(const std::string& header_buffer);
  ~HTTPHeader() override;

  // HTTPHeader(const HTTPHeader&) noexcept;
  // HTTPHeader& operator=(const HTTPHeader&) noexcept;

  // HTTPHeader(HTTPHeader&&) noexcept;
  // HTTPHeader& operator=(HTTPHeader&&) noexcept;

  std::string Get(const std::string& field) const noexcept {
    auto it = headers.find(field);
    if (it == headers.end()) return std::string();
    return it->second;
  }
  void Set(const std::string& field, const std::string& value) noexcept {
    headers.insert({field, value});
  }
  void Set(std::initializer_list<std::pair<std::string, std::string>>&
               headers) noexcept {
    for (const auto& header : headers) {
      this->headers.insert(header);
    }
  }

  std::string ToString() const noexcept override;

 private:
  std::unordered_map<std::string, std::string> headers;
};
}
}

#endif /* COLLIE_HTTP_HTTP_HEADER_H_ */
