#ifndef COLLIE_HTTP_HTTP_BASE_H_
#define COLLIE_HTTP_HTTP_BASE_H_

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace collie {
namespace http {

enum class Method {
  GET,
  POST,
  HEAD,
  OPTION,
  DELETE,
  PUT,
  TRACE,
  CONNECT,
  UNKNOWN
};

static const std::vector<std::pair<Method, std::string>> Methods{
    {Method::GET, "GET"},        {Method::POST, "POST"},
    {Method::HEAD, "HEAD"},      {Method::OPTION, "OPTION"},
    {Method::DELETE, "DELETE"},  {Method::PUT, "PUT"},
    {Method::TRACE, "TRACE"},    {Method::CONNECT, "CONNECT"},
    {Method::UNKNOWN, "UNKNOWN"}};

inline std::string to_string(Method method) noexcept {
  for (auto& m : Methods)
    if (m.first == method) return m.second;

  return "UNKNOWN";
}

inline Method to_http_method(const std::string& str) noexcept {
  std::string method(str);
  std::transform(method.begin(), method.end(), method.begin(),
                 [](auto& c) { return std::toupper(c); });
  // Gets upper string
  for (auto& m : Methods)
    if (m.second == method) return m.first;

  return Method::UNKNOWN;
}
}
}

#endif /* COLLIE_HTTP_HTTP_BASE_H_ */
