#ifndef COLLIE_UDP_UDPSOCKET_H_
#define COLLIE_UDP_UDPSOCKET_H_

#include <memory>
#include "../descriptor.h"

namespace collie {

class InetAddress;
enum class IP;

namespace udp {

class UDPSocket : public Descriptor {
 public:
  explicit UDPSocket(std::shared_ptr<InetAddress>);
  UDPSocket(const UDPSocket &) = delete;
  UDPSocket &operator=(const UDPSocket &) = delete;
  ~UDPSocket() override;

  int Get() const override { return fd_; }

  // server
  void Listen();
  void ListenV4();
  void ListenV6();

  // client
  void Connect(const IP);  // only socket, UDP have NO Connection
  void ConnectV4();
  void ConnectV6();

 private:
  int fd_;
  std::shared_ptr<InetAddress> address_;
};
}
}

#endif /* COLLIE_UDP_UDPSOCKET_H_ */
