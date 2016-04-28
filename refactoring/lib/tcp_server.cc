#include "../inc/tcp_server.h"
#include "../inc/inet_address.h"
#include "../inc/logger.h"
#include <sys/socket.h>

namespace collie {

TcpServer::TcpServer() noexcept : req_handler_(nullptr),
                                  host_address_(nullptr) {}
TcpServer::~TcpServer() noexcept {}

TcpServer& TcpServer::Listen(const Port port,
                             const char* host) throw(TcpException) {
  host_address_ = InetAddress::GetInetAddress(host, port);
  return Listen(host_address_);
}

TcpServer& TcpServer::Listen(Address host_address) throw(TcpException) {
  host_address_ = host_address;
  auto ip_family = AF_INET;
  if (host_address_->ip_version() == IPFamily::IPv6) {
    ip_family = AF_INET6;
  }
  local_fd_ = ::socket(ip_family, SOCK_STREAM, IPPROTO_TCP);
  if (local_fd_ == -1) {
    throw TcpException("TCP init");
  }
  int err_code = 0;

  if (host_address_->ip_version() == IPFamily::IPv4) {
    sockaddr_in* ipv4_addr = (sockaddr_in*)host_address->address();
    err_code = ::bind(local_fd_, (sockaddr*)ipv4_addr, sizeof(*ipv4_addr));

  } else {
    sockaddr_in6* ipv6_addr = (sockaddr_in6*)host_address->address();
    err_code = ::bind(local_fd_, (sockaddr*)ipv6_addr, sizeof(*ipv6_addr));
  }
  if (err_code == -1) {
    throw TcpException("TCP bind");
  }

  err_code = ::listen(local_fd_, SOMAXCONN);
  if (err_code == -1) {
    throw TcpException("TCP listen");
  }

  return *this;
}

void TcpServer::Start(const bool is_loop) throw(TcpException) {
  Accept();
  while (is_loop) {
    Accept();
  }
}

void TcpServer::Accept() throw(TcpException) {
  sockaddr client_address;
  socklen_t client_address_len = sizeof(client_address);
  int peer_fd = ::accept4(local_fd_, &client_address, &client_address_len, 0);
  if (peer_fd == -1) {
    throw TcpException("TCP accept");
  }
  TcpStream tcp_stream(host_address_, nullptr, peer_fd);
  req_handler_(tcp_stream);
}
}
