#ifndef COLLIE_TCP_SOCKET_H_
#define COLLIE_TCP_SOCKET_H_

#include <unistd.h>
#include <memory>
#include "../base/InetAddress.h"
#include "../util/NonCopyable.h"

namespace collie {
namespace tcp {

class TCPSocket final : public util::NonCopyable {
 public:
  TCPSocket(const int fd, std::shared_ptr<const InetAddress> local_addr,
            std::shared_ptr<const InetAddress> peer_addr) noexcept
      : local_address(local_addr),
        peer_address(peer_addr),
        fd(fd < 3 ? throw : fd) {}
  ~TCPSocket();

  int Descriptor() const noexcept { return fd; }

  static std::unique_ptr<TCPSocket> Listen(
      std::shared_ptr<InetAddress> local_address);
  std::unique_ptr<TCPSocket> Accept(bool is_block = true) const;

  static std::unique_ptr<TCPSocket> Connect(
      std::shared_ptr<InetAddress> peer_address);
  std::shared_ptr<const InetAddress> LocalAddress() const noexcept {
    return local_address;
  }
  std::shared_ptr<const InetAddress> PeerAddress() const noexcept {
    return peer_address;
  }

 private:
  std::shared_ptr<const InetAddress> local_address;
  std::shared_ptr<const InetAddress> peer_address;

  int fd;
};
}
}

#endif /* COLLIE_TCP_SOCKET_H_ */
