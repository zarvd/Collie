#ifndef COLLIE_TCP_TCPACCEPTOR_H_
#define COLLIE_TCP_TCPACCEPTOR_H_

#include <memory>
#include <functional>
#include <vector>

namespace collie {

class InetAddress;
class Descriptor;

namespace event {
class Channel;
class EventLoop;
}

namespace tcp {

class TCPSocket;

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
  void set_thread_num(const size_t thread_num);
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

  size_t thread_num_;
  std::shared_ptr<InetAddress> local_address_;
  std::shared_ptr<TCPSocket> tcp_socket_;  // listen socket
  std::vector<event::EventLoop> eventloops_;
  AcceptCallback accept_callback_;
};
}
}

#endif /* COLLIE_TCP_TCPACCEPTOR_H_ */
