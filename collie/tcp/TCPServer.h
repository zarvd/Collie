#pragma once

#include <collie/NonCopyable.h>
#include <collie/String.h>
#include <collie/tcp/TCPStream.h>
#include <functional>
#include <memory>

namespace collie {

class TCPServer : public NonCopyable {
 public:
  using RequestHandler = std::function<void(TCPStream &)>;

  TCPServer() noexcept : req_handler(nullptr) {}
  ~TCPServer();

  TCPServer &Listen(const unsigned port, const std::string &host = "0.0.0.0");
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
