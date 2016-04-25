#include <netdb.h>
#include <string.h>
#include "../inc/inet_address.h"
#include <iostream>

namespace collie {

InetAddress::~InetAddress() noexcept {}

std::shared_ptr<InetAddress> InetAddress::GetInetAddress(
    const Host &host, const Port port) throw(std::runtime_error) {
  addrinfo *addr_info;
  int err_code = ::getaddrinfo(host.c_str(), nullptr, nullptr, &addr_info);
  if (err_code != 0) {
    throw std::runtime_error(std::string("Cannot parse host: ") +
                             ::gai_strerror(err_code));
  }

  char hostname[NI_MAXHOST];
  err_code = getnameinfo(addr_info->ai_addr, addr_info->ai_addrlen, hostname,
                         NI_MAXHOST, NULL, 0, 0);
  if (err_code != 0) {
    throw std::runtime_error(std::string("Cannot parse host: ") +
                             ::gai_strerror(err_code));
  }

  std::shared_ptr<InetAddress> inet_address = nullptr;

  if (addr_info->ai_family == AF_INET) {
    inet_address = std::make_shared<IPv4Address>();
    char host_address[INET_ADDRSTRLEN];
    sockaddr_in *addr = ((sockaddr_in *)addr_info->ai_addr);
    if (!inet_ntop(AF_INET, &addr->sin_addr, host_address, INET_ADDRSTRLEN)) {
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }
    inet_address->host_ = host_address;

  } else if (addr_info->ai_family == AF_INET6) {
    inet_address = std::make_shared<IPv6Address>();
    char host_address[INET6_ADDRSTRLEN];
    sockaddr_in6 *addr = ((sockaddr_in6 *)addr_info->ai_addr);
    if (!inet_ntop(AF_INET6, &addr->sin6_addr, host_address,
                   INET6_ADDRSTRLEN)) {
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }
    inet_address->host_ = host_address;

  } else {
    ::freeaddrinfo(addr_info);
    throw std::runtime_error("Cannot parse host: Unknown family");
  }
  inet_address->port_ = port;
  inet_address->address_ = addr_info->ai_addr;

  ::freeaddrinfo(addr_info);

  return inet_address;
}
}
