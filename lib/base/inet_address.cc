#include <netdb.h>
#include <string.h>
#include "../inc/inet_address.h"

namespace collie {

InetAddress::InetAddress(const Host &host, const Port &port,
                         const Address address,
                         const IPFamily &ip_family) noexcept
    : host(host),
      port(port),
      address(address),
      ip_family(ip_family) {}

InetAddress::~InetAddress() noexcept {}

std::shared_ptr<InetAddress> InetAddress::GetInetAddress(
    const Host &host, const Port port) throw(std::runtime_error) {
  // Gets address info
  addrinfo *addr_info;
  int err_code = ::getaddrinfo(host.c_str(), nullptr, nullptr, &addr_info);
  if (err_code != 0) {
    throw std::runtime_error(std::string("Cannot parse host: ") +
                             ::gai_strerror(err_code));
  }

  std::shared_ptr<InetAddress> inet_address;

  if (addr_info->ai_family == AF_INET) {
    // IPv4
    char host_address[INET_ADDRSTRLEN];
    sockaddr_in *addr = ((sockaddr_in *)addr_info->ai_addr);
    addr->sin_port = ::htons(port);
    if (!inet_ntop(AF_INET, &addr->sin_addr, host_address, INET_ADDRSTRLEN)) {
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }
    inet_address = std::make_shared<InetAddress>(
        host_address, port, addr_info->ai_addr, IPFamily::IPv4);

  } else if (addr_info->ai_family == AF_INET6) {
    // IPv6
    char host_address[INET6_ADDRSTRLEN];
    sockaddr_in6 *addr = ((sockaddr_in6 *)addr_info->ai_addr);
    addr->sin6_port = ::htons(port);
    if (!inet_ntop(AF_INET6, &addr->sin6_addr, host_address,
                   INET6_ADDRSTRLEN)) {
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }
    inet_address = std::make_shared<InetAddress>(
        host_address, port, addr_info->ai_addr, IPFamily::IPv6);

  } else {
    ::freeaddrinfo(addr_info);
    throw std::runtime_error("Cannot parse host: Unknown family");
  }
  ::freeaddrinfo(addr_info);

  return inet_address;
}

InetAddress::AddressV4 InetAddress::GetIPv4Address() const
    throw(std::runtime_error) {
  if (ip_family != IPFamily::IPv4) {
    throw std::runtime_error("Cannot transfer the address to IPv4 address");
  }
  return AddressV4(address);
}
InetAddress::AddressV6 InetAddress::GetIPv6Address() const
    throw(std::runtime_error) {
  if (ip_family != IPFamily::IPv6) {
    throw std::runtime_error("Cannot transfer the address to IPv6 address");
  }
  return AddressV6(address);
}
}
