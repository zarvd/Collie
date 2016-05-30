#ifndef COLLIE_ASYNC_TCP_SERVER_H_
#define COLLIE_ASYNC_TCP_SERVER_H_

#include <memory>
#include <functional>
#include "util/noncopyable.h"
#include "exception.h"

namespace collie {

class InetAddress;
class AsyncTcpStream;
class TcpSocket;
class EventThreadPool;

class AsyncTcpServer : public util::NonCopyable {
 public:
  using Address = std::shared_ptr<InetAddress>;
  using Port = unsigned;
  using Host = std::string;
  using RequestHandler = std::function<void(std::shared_ptr<AsyncTcpStream>)>;

  AsyncTcpServer() noexcept;
  ~AsyncTcpServer() noexcept;

  AsyncTcpServer& Listen(const Port,
                         const Host& = "0.0.0.0") throw(TcpException);
  AsyncTcpServer& Listen(Address host_address) throw(TcpException);

  AsyncTcpServer& SetRequestHandler(const RequestHandler& handler) noexcept {
    req_handler_ = handler;
    return *this;
  };
  AsyncTcpServer& SetRequestHandler(RequestHandler&& handler) noexcept {
    req_handler_ = std::move(handler);
    return *this;
  }

  void SetLoop(std::shared_ptr<EventThreadPool> pool) noexcept;

 private:
  void Accept() throw(TcpException);

  std::shared_ptr<TcpSocket> socket_fd_;
  std::shared_ptr<AsyncTcpStream> socket_stream_;
  RequestHandler req_handler_;
  Address host_address_;
  std::shared_ptr<EventThreadPool> event_thread_pool_;
};
}

#endif /* COLLIE_ASYNC_TCP_SERVER_H_ */
