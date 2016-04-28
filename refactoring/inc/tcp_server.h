#ifndef COLLIE_TCP_SERVER_H_
#define COLLIE_TCP_SERVER_H_

#include <memory>
#include <functional>
#include "util/noncopyable.h"
#include "exception.h"

namespace collie {

class InetAddress;
class TcpStream;
class TcpSocket;

class TcpServer : public NonCopyable {
 public:
  using Address = std::shared_ptr<InetAddress>;
  using Port = unsigned;
  using Host = std::string;
  using RequestHandler = std::function<void(TcpStream&)>;

  TcpServer() noexcept;
  ~TcpServer() noexcept;

  TcpServer& Listen(const Port port,
                    const Host& host = "0.0.0.0") throw(TcpException);
  TcpServer& Listen(Address host_address) throw(TcpException);

  void Start(const bool is_loop = true) throw(TcpException);

  TcpServer& SetRequestHandler(const RequestHandler& handler) noexcept {
    req_handler_ = handler;
    return *this;
  };
  TcpServer& SetRequestHandler(RequestHandler&& handler) noexcept {
    req_handler_ = std::move(handler);
    return *this;
  }

 protected:
  void Accept() throw(TcpException);

  std::shared_ptr<TcpSocket> socket_fd_;
  RequestHandler req_handler_;
};
}

#endif /* COLLIE_TCP_SERVER_H_ */
