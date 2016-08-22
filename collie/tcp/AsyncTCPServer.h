#pragma once

#include <collie/EventThreadPool.h>
#include <collie/String.h>
#include <collie/tcp/AsyncTCPStream.h>
#include <collie/tcp/TCPSocket.h>
#include <collie/NonCopyable.h>
#include <functional>
#include <memory>

namespace collie {

class AsyncTCPServer : public NonCopyable {
 public:
  using RequestHandler = std::function<void(std::shared_ptr<AsyncTCPStream>)>;

  explicit AsyncTCPServer(std::shared_ptr<EventThreadPool>) noexcept;
  ~AsyncTCPServer();

  AsyncTCPServer& Listen(const unsigned port, const std::string& = "0.0.0.0");
  AsyncTCPServer& Listen(std::shared_ptr<InetAddress> host_address);

  AsyncTCPServer& SetRequestHandler(const RequestHandler& handler) noexcept {
    req_handler = handler;
    return *this;
  };
  AsyncTCPServer& SetRequestHandler(RequestHandler&& handler) noexcept {
    req_handler = std::move(handler);
    return *this;
  }

  void SetEventLoop(std::shared_ptr<EventThreadPool> pool) noexcept {
    event_thread_pool = pool;
  }
  void Start();

 private:
  void Accept();

  std::unique_ptr<TCPSocket> socket;
  RequestHandler req_handler;
  std::shared_ptr<const InetAddress> host_address;
  std::shared_ptr<EventThreadPool> event_thread_pool;
};
}
