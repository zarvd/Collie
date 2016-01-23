#ifndef COLLIE_TCP_TCPSERVER_H_
#define COLLIE_TCP_TCPSERVER_H_

#include <thread>
#include <memory>
#include <string>
#include <unordered_set>
#include "../../include/inet_address.h"

namespace collie {

namespace tcp {

class TCPSocket;
class TCPIOStream;

// Non blocking TCP server
// it creates a `EventLoopThreadPool` and a basic listening `Channel`
// every loop listens this `Channel` and accepts new connections
// `connected_callback_` is called when new connection is accepted
// `on_message_callback_` is called when new connection is ready to READ
// a unique `TCPIOStream` is created and it do all I/O operations
// Usage:
//   TCPServer server;
//   server.Bind("0.0.0.0", port);
//   server.set_thread_num(8);
//   server.set_on_message_callback([](auto iostream) { ... });
//   server.Start();
class TCPServer {
 public:
  using OnMessageCallback =
      std::function<void(std::shared_ptr<TCPIOStream> iostream)>;
  using ConnectedCallback = std::function<void()>;

  TCPServer();
  TCPServer(const TCPServer &) = delete;
  TCPServer &operator=(const TCPServer &) = delete;
  ~TCPServer();

  void Start();
  void Bind(const std::string &host, const unsigned port);
  void set_thread_num(const size_t thread_num = 1) { thread_num_ = thread_num; }
  // setter
  void set_connected_callback(const ConnectedCallback &cb) {
    connected_callback_ = cb;
  }
  void set_on_message_callback(const OnMessageCallback &cb) {
    on_message_callback_ = cb;
  }
  // getter
  size_t thread_num() const { return thread_num_; }
  std::string host() const { return host_; }
  unsigned port() const { return port_; }
  InetAddress local_address() const { return *local_address_; }

 private:
  void HandleAccept();
  void HandleError();
  void HandleConnection(std::shared_ptr<TCPSocket> conn_socket);

  std::shared_ptr<TCPSocket> listen_socket_;
  size_t thread_num_;
  std::string host_;
  unsigned port_;
  std::shared_ptr<InetAddress> local_address_;
  ConnectedCallback connected_callback_;
  OnMessageCallback on_message_callback_;
};
}
}

#endif /* COLLIE_TCP_TCPSERVER_H_ */
