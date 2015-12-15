#ifndef COLLIE_TCP_TCPACCEPTOR_H_
#define COLLIE_TCP_TCPACCEPTOR_H_

#include <memory>
#include <functional>
#include <vector>

namespace collie {

class InetAddress;

namespace event {
class Channel;
}

namespace tcp {

class TCPSocket;

// `TCPAcceptor` generates TCP listening `Channel`
// it creates a `TCPSocket` in constructor
// Usage:
//   TCPAcceptor acceptor(local_address);
//   acceptor.BindAndListen();
//   acceptor.set_accept_callback([] (auto conn_socket) {...});
//   auto channel = acceptor->GetBaseChannel();
class TCPAcceptor {
 public:
  using AcceptCallback =
      std::function<void(std::shared_ptr<TCPSocket> conn_socket)>;

  explicit TCPAcceptor(std::shared_ptr<InetAddress>) noexcept;
  TCPAcceptor(const TCPAcceptor &) = delete;
  TCPAcceptor &operator=(const TCPAcceptor &) = delete;
  ~TCPAcceptor() noexcept;

  void BindAndListen() const;

  // setter
  void set_accept_callback(const AcceptCallback &cb) noexcept {
    accept_callback_ = cb;
  }
  void set_accept_callback(const AcceptCallback &&cb) noexcept {
    accept_callback_ = std::move(cb);
  }

  // getter
  std::shared_ptr<event::Channel> GetBaseChannel();

 private:
  void HandleRead();
  void HandleError();

  std::shared_ptr<InetAddress> local_address_;
  std::shared_ptr<TCPSocket> tcp_socket_;  // listening socket
  AcceptCallback accept_callback_;
};
}
}

#endif /* COLLIE_TCP_TCPACCEPTOR_H_ */
