#ifndef COLLIE_TCP_TCPSERVER_H_
#define COLLIE_TCP_TCPSERVER_H_

#include <thread>
#include <memory>
#include <string>

namespace collie {

namespace event {
class EventLoopThreadPool;
}

class InetAddress;

namespace tcp {

class TCPAcceptor;
class TCPConnection;
class TCPSocket;

class TCPServer {
 public:
  using OnMessageCallback = std::function<void(std::shared_ptr<TCPConnection>)>;
  using ConnectedCallback = std::function<void()>;

  TCPServer();
  TCPServer(const TCPServer &) = delete;
  TCPServer &operator=(const TCPServer &) = delete;
  ~TCPServer();

  void Start();
  void Bind(const std::string &host, const unsigned port);
  void set_thread_num(const size_t thread_num = 1) { thread_num_ = thread_num; }
  // setter
  void setConnectedCallback(const ConnectedCallback &cb) {
    connected_callback_ = cb;
  }
  void setConnectedCallback(const ConnectedCallback &&cb) {
    connected_callback_ = std::move(cb);
  }
  void setOnMessageCallback(const OnMessageCallback &cb) {
    on_message_callback_ = cb;
  }
  void setOnMessageCallback(const OnMessageCallback &&cb) {
    on_message_callback_ = std::move(cb);
  }
  // getter
  unsigned port() const { return port_; }

 private:
  void NewConnection(std::shared_ptr<TCPSocket> conn_socket);

  size_t thread_num_;
  std::string host_;
  unsigned port_;
  std::shared_ptr<InetAddress> local_address_;
  std::unique_ptr<event::EventLoopThreadPool> eventloop_threadpool_;
  std::unique_ptr<TCPAcceptor> acceptor_;
  ConnectedCallback connected_callback_;
  OnMessageCallback on_message_callback_;
};
}
}

#endif /* COLLIE_TCP_TCPSERVER_H_ */
