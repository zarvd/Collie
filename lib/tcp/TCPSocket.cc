#include "../../inc/tcp/TCPSocket.h"
#include <cstring>
#include "../../inc/base/InetAddress.h"
#include "../../inc/base/Logger.h"

namespace collie {
namespace tcp {

TCPSocket::~TCPSocket() {}

std::unique_ptr<TCPSocket> TCPSocket::Listen(
    std::shared_ptr<InetAddress> address) {
  int fd = -1;
  int ret = 0;
  if (address->Family() == IPFamily::IPv4) {
    fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  } else {
    fd = ::socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
  }
  if (fd == -1) throw std::runtime_error(::strerror(errno));

  ret = ::bind(fd, address->Address(), sizeof(*address->Address()));
  if (ret == -1) {
    ::close(fd);
    throw std::runtime_error(::strerror(errno));
  }

  ret = ::listen(fd, SOMAXCONN);
  if (ret == -1) {
    ::close(fd);
    throw std::runtime_error(::strerror(errno));
  }

  return std::make_unique<TCPSocket>(fd, address, nullptr);
}

std::unique_ptr<TCPSocket> TCPSocket::Connect(
    std::shared_ptr<InetAddress> peer_address,
    std::shared_ptr<InetAddress> local_address) {
  if (local_address && local_address->Family() != peer_address->Family()) {
    throw std::runtime_error(
        "Peer address family doesn't match local address family");
  }
  int fd;
  int ret = 0;
  if (peer_address->Family() == IPFamily::IPv4) {
    fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) throw std::runtime_error(::strerror(errno));
  } else {
    fd = ::socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) throw std::runtime_error(::strerror(errno));
  }

  if (local_address) {
    ret =
        ::bind(fd, local_address->Address(), sizeof(*local_address->Address()));
    if (ret == -1) {
      ::close(fd);
      throw std::runtime_error(::strerror(errno));
    }
  }

  ret =
      ::connect(fd, peer_address->Address(), sizeof(*peer_address->Address()));

  if (ret == -1) {
    ::close(fd);
    throw std::runtime_error(::strerror(errno));
  }
  return std::make_unique<TCPSocket>(fd, local_address, peer_address);
}

std::unique_ptr<TCPSocket> TCPSocket::Accept(bool) const {
  auto client_address = std::make_unique<sockaddr>();
  socklen_t client_address_len = sizeof(*client_address);

  // Accepts new client
  int peer_fd = ::accept4(fd, client_address.get(), &client_address_len, 0);
  if (peer_fd == -1) {
    LOG(DEBUG) << "TCP Socket Accept: " << ::strerror(errno);
    return nullptr;
  }

  // Creates connection socket
  auto peer_address = std::make_shared<InetAddress>(std::move(client_address));
  auto socket = std::make_unique<TCPSocket>(peer_fd, nullptr, peer_address);

  return socket;
}
}
}
