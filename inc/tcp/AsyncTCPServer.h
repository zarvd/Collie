#ifndef COLLIE_TCP_ASYNC_TCP_SERVER_H_
#define COLLIE_TCP_ASYNC_TCP_SERVER_H_

#include <functional>
#include <memory>
#include "../base/String.h"
#include "../util/NonCopyable.h"

namespace collie {

class InetAddress;
class EventThreadPool;

namespace tcp {

class AsyncTCPStream;
class TCPSocket;

class AsyncTCPServer : public util::NonCopyable {
 public:
  using RequestHandler = std::function<void(std::shared_ptr<AsyncTCPStream>)>;

  AsyncTCPServer() noexcept;
  ~AsyncTCPServer() noexcept;

  AsyncTCPServer& Listen(const unsigned port, const String& = "0.0.0.0");
  AsyncTCPServer& Listen(std::shared_ptr<InetAddress> host_address);

  AsyncTCPServer& SetRequestHandler(const RequestHandler& handler) noexcept {
    req_handler = handler;
    return *this;
  };
  AsyncTCPServer& SetRequestHandler(RequestHandler&& handler) noexcept {
    req_handler = std::move(handler);
    return *this;
  }

  void SetEventLoop(std::shared_ptr<EventThreadPool> pool) noexcept;
  void Start() noexcept;

 private:
  void Accept();

  std::unique_ptr<TCPSocket> socket;
  // std::shared_ptr<AsyncTCPStream> socket_stream;
  RequestHandler req_handler;
  std::shared_ptr<const InetAddress> host_address;
  std::shared_ptr<EventThreadPool> event_thread_pool;
};
}
}

#endif /* COLLIE_TCP_ASYNC_TCP_SERVER_H_ */
