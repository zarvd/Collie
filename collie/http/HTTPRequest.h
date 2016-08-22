#ifndef COLLIE_HTTP_HTTP_REQUEST_H_
#define COLLIE_HTTP_HTTP_REQUEST_H_

#include <memory>
#include "../../inc/util/NonCopyable.h"
#include "HTTPBase.h"
#include "HTTPHeader.h"

namespace collie {
namespace http {

class HTTPRequest final : public util::NonCopyable {
 public:
  HTTPRequest(
      const Method& method, const std::string& url,
      const std::shared_ptr<HTTPHeader> header,
      const std::initializer_list<std::pair<std::string, std::string>>& query,
      const std::string& protocol = "HTTP/1.1");
  explicit HTTPRequest(const std::string& http_message);
  ~HTTPRequest() override;

  std::shared_ptr<const HTTPHeader> Header() const noexcept { return header; }

  Method Method() const noexcept { return method; }
  std::string Url() const noexcept { return url; }
  bool HasQuery(const std::string& field) const noexcept {
    return query_params.find(field) != query_params.end();
  }
  std::string Query(const std::string& field) const noexcept {
    const auto it = query_params.find(field);
    if (it != query_params.end()) {
      return it->second;
    } else {
      return "";
    }
  }
  std::vector<std::string> QueryArray(const std::string& field) const noexcept {
    // FIXME could be duplicated
    const auto pair = query_params.equal_range(field);
    std::vector<std::string> queries;
    for (auto it = pair.first; it != pair.second; ++it) {
      queries.emplace_back(it->second);
    }

    return queries;
  }

  bool HasForm(const std::string& field) const noexcept;
  std::string Form(const std::string& field) const noexcept;

 private:
  http::Method method;
  std::string url;
  std::shared_ptr<HTTPHeader> header;
  std::unordered_multimap<std::string, std::string> query_params;
  std::string protocol;
};
}
}

#endif /* COLLIE_HTTP_HTTP_REQUEST_H_ */
