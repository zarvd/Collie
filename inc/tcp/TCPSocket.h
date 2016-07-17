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
  TCPSocket() noexcept : local_address(nullptr),
                         peer_address(nullptr),
                         fd(-1) {}
  ~TCPSocket() noexcept {}

  int Descriptor() const noexcept { return fd; }

  void Listen(std::shared_ptr<InetAddress> local_address);
  std::unique_ptr<TCPSocket> Accept(bool is_block = true) const;

  void Connect(std::shared_ptr<InetAddress> peer_address);
  std::shared_ptr<InetAddress> LocalAddress() const noexcept {
    return local_address;
  }
  std::shared_ptr<InetAddress> PeerAddress() const noexcept {
    return peer_address;
  }
  void Close() noexcept {
    if (fd != -1) {
      ::close(fd);
      fd = -1;
    }
  }

 protected:
  std::shared_ptr<InetAddress> local_address;
  std::shared_ptr<InetAddress> peer_address;

  int fd;
};
}
}

#endif /* COLLIE_TCP_SOCKET_H_ */
