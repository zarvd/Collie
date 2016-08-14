#ifndef COLLIE_HTTP_HTTP_REQUEST_H_
#define COLLIE_HTTP_HTTP_REQUEST_H_

#include <memory>
#include "../../inc/util/NonCopyable.h"
#include "HTTPBase.h"
#include "HTTPHeader.h"

namespace collie {
namespace http {

class HTTPRequest : public util::NonCopyable {
 public:
  explicit HTTPRequest(const std::string&);
  ~HTTPRequest();

  std::shared_ptr<const HTTPHeader> Header() const noexcept { return header; }

  Method Method() const noexcept { return method; }
  std::string Url() const noexcept { return url; }
  bool HasQuery(const std::string& field) const noexcept;
  std::string Query(const std::string& field) const noexcept;
  bool HasForm(const std::string& field) const noexcept;
  std::string Form(const std::string& field) const noexcept;

 private:
  std::shared_ptr<HTTPHeader> header;
  http::Method method;
  std::string url;
  std::string protocol;
  std::unordered_multimap<std::string, std::string> query_params;
};
}
}

#endif /* COLLIE_HTTP_HTTP_REQUEST_H_ */
