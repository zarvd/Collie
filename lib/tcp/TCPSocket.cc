#include "../../inc/tcp/TCPSocket.h"
#include "../../inc/base/InetAddress.h"
#include "../../inc/base/Logger.h"

namespace collie {
namespace tcp {

// TCPSocket::~TCPSocket() noexcept {}

void TCPSocket::Listen(std::shared_ptr<InetAddress> address) {
  if (fd > 0) {
    Close();
  }
  auto ip_family = AF_INET;
  if (address->Family() == IPFamily::IPv6) {
    ip_family = AF_INET6;
  }

  fd = ::socket(ip_family, SOCK_STREAM, IPPROTO_TCP);
  if (fd == -1) {
    throw std::system_error();
  }

  int err_code = 0;
  if (address->Family() == IPFamily::IPv4) {
    auto ipv4_addr = address->AddressV4();
    err_code = ::bind(fd, (sockaddr *)ipv4_addr, sizeof(*ipv4_addr));

  } else {
    auto ipv6_addr = address->AddressV6();
    err_code = ::bind(fd, (sockaddr *)ipv6_addr, sizeof(*ipv6_addr));
  }
  if (err_code == -1) {
    throw std::system_error();
  }

  err_code = ::listen(fd, SOMAXCONN);
  if (err_code == -1) {
    throw std::system_error();
  }

  local_address = address;
}

void TCPSocket::Connect(std::shared_ptr<InetAddress> address) {
  if (fd > 0) {
    Close();
  }
  auto ip_family = AF_INET;
  if (address->Family() == IPFamily::IPv6) {
    ip_family = AF_INET6;
  }

  fd = ::socket(ip_family, SOCK_STREAM, IPPROTO_TCP);
  if (fd == -1) {
    throw std::system_error();
  }

  int err_code = 0;
  if (address->Family() == IPFamily::IPv4) {
    auto ipv4_addr = address->AddressV4();
    err_code = ::connect(fd, (sockaddr *)ipv4_addr, sizeof(*ipv4_addr));
  } else {
    auto ipv6_addr = address->AddressV6();
    err_code = ::connect(fd, (sockaddr *)ipv6_addr, sizeof(*ipv6_addr));
  }
  if (err_code == -1) {
    throw std::system_error();
  }

  peer_address = address;
}

std::unique_ptr<TCPSocket> TCPSocket::Accept(bool) const {
  sockaddr *client_address = new sockaddr;
  socklen_t client_address_len = sizeof(*client_address);
  int peer_fd = ::accept4(fd, client_address, &client_address_len, 0);
  if (peer_fd == -1) {
    LOG(DEBUG) << "TCP Socket Accept: " << ::strerror(errno);
    return nullptr;
  }
  auto socket = std::make_unique<TCPSocket>();
  socket->fd = peer_fd;
  socket->peer_address = std::make_shared<InetAddress>(client_address);
  return socket;
}
}
}
