#ifndef COLLIE_TCP_SERVER_H_
#define COLLIE_TCP_SERVER_H_

#include <functional>
#include <memory>
#include "../base/String.h"
#include "../util/NonCopyable.h"
#include "TCPSocket.h"

namespace collie {

namespace tcp {

class TCPStream;

class TCPServer : public util::NonCopyable {
 public:
  using RequestHandler = std::function<void(TCPStream &)>;

  TCPServer() noexcept : req_handler(nullptr) {}
  ~TCPServer() noexcept;

  TCPServer &Listen(const unsigned port, const String &host = "0.0.0.0");
  TCPServer &Listen(std::shared_ptr<InetAddress> host_address);

  void Start(const bool is_loop = true);

  TCPServer &SetRequestHandler(const RequestHandler &handler) noexcept {
    req_handler = handler;
    return *this;
  };
  TCPServer &SetRequestHandler(RequestHandler &&handler) noexcept {
    req_handler = std::move(handler);
    return *this;
  }

 protected:
  void Accept();

  std::unique_ptr<TCPSocket> socket;
  RequestHandler req_handler;
};
}
}

#endif /* COLLIE_TCP_SERVER_H_ */
