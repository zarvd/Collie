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
  explicit UDPSocket(std::shared_ptr<InetAddress> local_address_);
  UDPSocket(const UDPSocket &) = delete;
  UDPSocket &operator=(const UDPSocket &) = delete;
  ~UDPSocket() override;

  void Create() override;
  void Close() override;

  // server
  void Listen();

  // client
  void Connect(const IP);  // only socket, UDP have NO Connection

 private:
  void ListenV4();
  void ListenV6();
  void ConnectV4();
  void ConnectV6();

  void CreateImpl() noexcept;
  void CloseImpl() noexcept;

  std::shared_ptr<InetAddress> local_address_;
};
}
}

#endif /* COLLIE_UDP_UDPSOCKET_H_ */
