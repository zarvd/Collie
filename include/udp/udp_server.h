#ifndef COLLIE_UDP_UDPSERVER_H_
#define COLLIE_UDP_UDPSERVER_H_

#include <string>
#include <memory>
#include <functional>

namespace collie {

class InetAddress;

namespace udp {

class UDPSocket;

class UDPServer {
 public:
  using ConnectCallback = std::function<void(
      const std::string &content, std::shared_ptr<InetAddress> remote_address)>;

  UDPServer();
  UDPServer(const UDPServer &) = delete;
  UDPServer &operator=(const UDPServer &) = delete;
  ~UDPServer();

  void set_connect_callback(const ConnectCallback &cb) {
    connect_callback_ = cb;
  }
  void set_connect_callback(const ConnectCallback &&cb) {
    connect_callback_ = std::move(cb);
  }
  void Bind(const std::string &host, const unsigned port);
  void Start();
  void Send(const std::string &content,
            std::shared_ptr<InetAddress> remote_address);

 private:
  std::unique_ptr<UDPSocket> socket_;
  std::shared_ptr<InetAddress> local_address_;
  ConnectCallback connect_callback_;
};
}
}

#endif /* COLLIE_UDP_UDPSERVER_H_ */
