#ifndef COLLIE_TCP_ASYNC_TCP_SERVER_H_
#define COLLIE_TCP_ASYNC_TCP_SERVER_H_

#include <functional>
#include <memory>
#include "../../inc/base/EventThreadPool.h"
#include "../../inc/tcp/AsyncTCPStream.h"
#include "../../inc/tcp/TCPSocket.h"
#include "../base/String.h"
#include "../util/NonCopyable.h"

namespace collie {
namespace tcp {

class AsyncTCPServer : public util::NonCopyable {
 public:
  using RequestHandler = std::function<void(std::shared_ptr<AsyncTCPStream>)>;

  AsyncTCPServer(std::shared_ptr<EventThreadPool>) noexcept;
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
}

#endif /* COLLIE_TCP_ASYNC_TCP_SERVER_H_ */
