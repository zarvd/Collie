#include "../inc/tcp_socket.h"
#include "../inc/logger.h"
#include "../inc/inet_address.h"

namespace collie {

TcpSocket::TcpSocket() noexcept {}
TcpSocket::~TcpSocket() noexcept {}

void TcpSocket::Listen(Address host_addr) throw(TcpException) {
  if (fd_ > 0) {
    Close();
  }
  auto ip_family = AF_INET;
  if (host_addr->ip_family() == IPFamily::IPv6) {
    ip_family = AF_INET6;
  }

  fd_ = ::socket(ip_family, SOCK_STREAM, IPPROTO_TCP);
  if (fd_ == -1) {
    throw TcpException("TCP Socket init");
  }

  int err_code = 0;
  if (host_addr->ip_family() == IPFamily::IPv4) {
    auto ipv4_addr = host_addr->GetIPv4Address();
    err_code = ::bind(fd_, (sockaddr *)ipv4_addr, sizeof(*ipv4_addr));

  } else {
    auto ipv6_addr = host_addr->GetIPv6Address();
    err_code = ::bind(fd_, (sockaddr *)ipv6_addr, sizeof(*ipv6_addr));
  }
  if (err_code == -1) {
    throw TcpException("TCP Socket bind");
  }

  err_code = ::listen(fd_, SOMAXCONN);
  if (err_code == -1) {
    throw TcpException("TCP Socket listen");
  }
  address_ = host_addr;
}

void TcpSocket::Connect(Address serv_addr) throw(TcpException) {
  if (fd_ > 0) {
    Close();
  }
  auto ip_family = AF_INET;
  if (serv_addr->ip_family() == IPFamily::IPv6) {
    ip_family = AF_INET6;
  }

  fd_ = ::socket(ip_family, SOCK_STREAM, IPPROTO_TCP);
  if (fd_ == -1) {
    throw TcpException("TCP Socket init");
  }

  int err_code = 0;
  if (serv_addr->ip_family() == IPFamily::IPv4) {
    auto ipv4_addr = serv_addr->GetIPv4Address();
    err_code = ::connect(fd_, (sockaddr *)ipv4_addr, sizeof(*ipv4_addr));
  } else {
    auto ipv6_addr = serv_addr->GetIPv6Address();
    err_code = ::connect(fd_, (sockaddr *)ipv6_addr, sizeof(*ipv6_addr));
  }
  if (err_code == -1) {
    throw TcpException("TCP Socket connect");
  }
  address_ = serv_addr;
}

std::shared_ptr<TcpSocket> TcpSocket::Accept() throw(TcpException) {
  sockaddr client_address;
  socklen_t client_address_len = sizeof(client_address);
  int peer_fd = ::accept4(fd_, &client_address, &client_address_len, 0);
  if (peer_fd == -1) {
    throw TcpException("TCP Socket accept");
  }
  auto socket = std::make_shared<TcpSocket>();
  socket->fd_ = peer_fd;
  socket->address_ = nullptr;  // FIXME
  return socket;
}
}
