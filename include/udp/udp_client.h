#ifndef COLLIE_UDP_UDPCLIENT_H_
#define COLLIE_UDP_UDPCLIENT_H_

#include <string>
#include <memory>
#include <functional>

namespace collie {

class InetAddress;

namespace udp {

class UDPSocket;

class UDPClient {
 public:
  using ConnectCallback = std::function<bool(
      const std::string &content, std::shared_ptr<InetAddress> remote_address)>;

  UDPClient();
  UDPClient(const UDPClient &) = delete;
  UDPClient &operator=(const UDPClient &) = delete;
  ~UDPClient();

  void set_connect_callback(const ConnectCallback &cb) {
    connect_callback_ = cb;
  }
  void set_connect_callback(const ConnectCallback &&cb) {
    connect_callback_ = std::move(cb);
  }
  void Connect(const std::string &host, const unsigned port,
               const std::string &greeting);
  void Send(const std::string &content,
            std::shared_ptr<InetAddress> remote_address);

 private:
  std::unique_ptr<UDPSocket> socket_;
  std::shared_ptr<InetAddress> remote_address_;
  ConnectCallback connect_callback_;
};
}
}

#endif /* COLLIE_UDP_UDPCLIENT_H_ */
