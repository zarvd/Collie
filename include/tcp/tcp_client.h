#ifndef COLLIE_TCP_TCPCLIENT_H_
#define COLLIE_TCP_TCPCLIENT_H_

#include <functional>
#include <memory>
#include <string>

namespace collie {

class InetAddress;

namespace tcp {

class TCPConnector;
class TCPSocket;
class TCPIOStream;

class TCPClient {
 public:
  using ConnectCallback =
      std::function<void(std::shared_ptr<TCPIOStream> iostream,
                         std::shared_ptr<InetAddress> local_address,
                         std::shared_ptr<InetAddress> peer_address)>;

  TCPClient();
  TCPClient(const TCPClient &) = delete;
  TCPClient &operator=(const TCPClient &) = delete;
  ~TCPClient();

  void set_connect_callback(const ConnectCallback &cb) {
    connect_callback_ = cb;
  }
  void set_connect_callback(const ConnectCallback &&cb) {
    connect_callback_ = std::move(cb);
  }
  void Connect(const std::string &host, const unsigned port,
               const size_t thread_num = 1, const size_t connect_num = 1);

 private:
  std::unique_ptr<TCPConnector> connector_;
  ConnectCallback connect_callback_;
  std::shared_ptr<InetAddress> peer_address_;
};
}
}

#endif /* COLLIE_TCP_TCPCLIENT_H_ */
