#pragma once

#include <collie/InetAddress.h>
#include <collie/NonCopyable.h>
#include <unistd.h>
#include <memory>

namespace collie {

class TCPSocket final : public NonCopyable {
 public:
  TCPSocket(const int fd, const std::shared_ptr<const InetAddress>& local_addr,
            const std::shared_ptr<const InetAddress>& peer_addr)
      : local_address(local_addr), peer_address(peer_addr), fd(fd) {
    if (fd < 3) {
      throw std::runtime_error("TCP socket file descriptor is invalid");
    }
  }
  ~TCPSocket();

  int Descriptor() const noexcept { return fd; }

  static std::unique_ptr<TCPSocket> Listen(
      std::shared_ptr<InetAddress> local_address);
  std::unique_ptr<TCPSocket> Accept(bool is_block = true) const;

  static std::unique_ptr<TCPSocket> Connect(
      std::shared_ptr<InetAddress> peer_address,
      std::shared_ptr<InetAddress> local_address = nullptr);
  std::shared_ptr<const InetAddress> LocalAddress() const noexcept {
    return local_address;
  }
  std::shared_ptr<const InetAddress> PeerAddress() const noexcept {
    return peer_address;
  }

 private:
  std::shared_ptr<const InetAddress> local_address;
  std::shared_ptr<const InetAddress> peer_address;

  const int fd;
};
}
