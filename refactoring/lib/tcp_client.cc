#include "../inc/tcp_client.h"
#include "../inc/inet_address.h"
#include "../inc/tcp_stream.h"
#include <sys/socket.h>

namespace collie {

void TcpClient::Connect(const Host &host, const Port &port,
                        const Request &req) throw(TcpException) {
  auto serv_addr = InetAddress::GetInetAddress(host, port);
  Connect(serv_addr, req);
}

void TcpClient::Connect(Address serv_addr,
                        const Request &req) throw(TcpException) {
  auto ip_family = AF_INET;
  if (serv_addr->ip_family() == IPFamily::IPv6) {
    ip_family = AF_INET6;
  }

  int peer_fd = ::socket(ip_family, SOCK_STREAM, IPPROTO_TCP);
  if (peer_fd == -1) {
    throw TcpException("TCP init");
  }

  int err_code = 0;
  if (serv_addr->ip_family() == IPFamily::IPv4) {
    auto ipv4_addr = serv_addr->GetIPv4Address();
    err_code = ::connect(peer_fd, (sockaddr *)ipv4_addr, sizeof(*ipv4_addr));
  } else {
    auto ipv6_addr = serv_addr->GetIPv6Address();
    err_code = ::connect(peer_fd, (sockaddr *)ipv6_addr, sizeof(*ipv6_addr));
  }
  if (err_code == -1) {
    throw TcpException("TCP connect");
  }

  TcpStream tcp_stream(nullptr, serv_addr, peer_fd);
  req(tcp_stream);
}
}
