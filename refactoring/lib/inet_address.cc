#include <netdb.h>
#include <string.h>
#include "../inc/inet_address.h"

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

  char hostname[100];
  err_code = getnameinfo(addr_info->ai_addr, addr_info->ai_addrlen, hostname,
                         NI_MAXHOST, NULL, 0, 0);
  if (err_code != 0) {
    throw std::runtime_error(std::string("Cannot parse host: ") +
                             ::gai_strerror(err_code));
  }

  const auto ip_family = addr_info->ai_family;

  ::freeaddrinfo(addr_info);

  std::shared_ptr<InetAddress> inet_address = nullptr;

  if (ip_family == AF_INET) {
    inet_address = std::make_shared<IPv4Address>();

    inet_address->host_ = hostname;
    inet_address->port_ = port;
    sockaddr_in *addr = new sockaddr_in;
    addr->sin_family = AF_INET;
    addr->sin_port = ::htons(port);
    if (::inet_pton(AF_INET, hostname, &(addr->sin_addr)) == -1) {
      delete addr;
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }
    inet_address->address_ = Address(addr);

  } else if (ip_family == AF_INET6) {
    inet_address = std::make_shared<IPv6Address>();

    inet_address->host_ = hostname;
    inet_address->port_ = port;

    sockaddr_in6 *addr = new sockaddr_in6;
    addr->sin6_family = AF_INET6;
    addr->sin6_port = ::htons(port);
    if (::inet_pton(AF_INET6, hostname, &(addr->sin6_addr)) == -1) {
      delete addr;
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }
    inet_address->address_ = Address(addr);

  } else {
    throw std::runtime_error("Cannot parse host: Unknown family");
  }

  return inet_address;
}
}
