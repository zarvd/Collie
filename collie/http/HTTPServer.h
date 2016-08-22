#ifndef COLLIE_HTTP_HTTP_SERVER_H_
#define COLLIE_HTTP_HTTP_SERVER_H_

#include <functional>
#include <memory>
#include "../../inc/base/String.h"
#include "../../inc/util/NonCopyable.h"
#include "../tcp/TCPServer.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"

namespace collie {
namespace http {

class HTTPServer : public util::NonCopyable {
 public:
  using RequestHandler =
      std::function<HTTPResponse*(std::shared_ptr<const HTTPRequest>)>;

  HTTPServer(const unsigned port, const std::string& host);
  ~HTTPServer();

  void Start();
  void Handle(const RequestHandler& handler) noexcept { req_handler = handler; }

 private:
  RequestHandler req_handler;
  const std::unique_ptr<tcp::TCPServer> tcp_server;
  const unsigned port;
  const std::string host;
};
}
}

#endif /* COLLIE_HTTP_HTTP_SERVER_H_ */
