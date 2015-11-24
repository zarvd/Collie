#ifndef COLLIE_TCP_TCPCONNECTOR_H_
#define COLLIE_TCP_TCPCONNECTOR_H_

#include <functional>
#include <memory>

namespace collie {

namespace event {

class ThreadPool;
}

class InetAddress;

namespace tcp {

class TCPSocket;

/**
 * Based on thread pool
 */
class TCPConnector {
 public:
  using ConnectCallback = std::function<void(std::shared_ptr<TCPSocket>)>;

  explicit TCPConnector(std::shared_ptr<InetAddress>);
  TCPConnector(const TCPConnector &) = delete;
  TCPConnector &operator=(const TCPConnector &) = delete;
  ~TCPConnector();

  void set_connect_callback(const ConnectCallback &cb) {
    connect_callback_ = cb;
  }
  void set_connect_callback(const ConnectCallback &&cb) {
    connect_callback_ = std::move(cb);
  }
  void Connect(const size_t thread_num, const size_t connect_num);
  void Disconnect();

 private:
  std::unique_ptr<event::ThreadPool> threadpool_;
  std::shared_ptr<InetAddress> remote_address_;
  ConnectCallback connect_callback_;
};
}
}

#endif /* COLLIE_TCP_TCPCONNECTOR_H_ */
