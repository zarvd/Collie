#include "../../inc/base/InetAddress.h"
#include <netdb.h>
// #include <string.h>

namespace collie {
namespace base {

// InetAddress::InetAddress(const String &host, const unsigned &port,
//                          const sockaddr *address,
//                          const IPFamily &family) noexcept : host(host),
//                                                             port(port),
//                                                             address(address),
//                                                             family(family) {}

InetAddress::~InetAddress() noexcept {
  if (address) delete address;
}

InetAddress InetAddress::GetInetAddress(const String &host,
                                        const unsigned port) {
  // Gets address info
  addrinfo *addr_info;
  int err_code = ::getaddrinfo(host.RawData(), nullptr, nullptr, &addr_info);
  if (err_code != 0) {
    throw std::runtime_error(std::string("Cannot parse host: ") +
                             ::gai_strerror(err_code));
  }

  InetAddress inet_address;
  inet_address.address = new sockaddr;
  inet_address.port = port;

  char host_address[INET6_ADDRSTRLEN];
  if (addr_info->ai_family == AF_INET) {
    // IPv4
    sockaddr_in *addr = ((sockaddr_in *)addr_info->ai_addr);
    addr->sin_port = ::htons(port);
    if (!inet_ntop(AF_INET, &addr->sin_addr, host_address, INET_ADDRSTRLEN)) {
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }

    inet_address.family = IPFamily::IPv4;
  } else if (addr_info->ai_family == AF_INET6) {
    // IPv6
    sockaddr_in6 *addr = ((sockaddr_in6 *)addr_info->ai_addr);
    addr->sin6_port = ::htons(port);
    if (!inet_ntop(AF_INET6, &addr->sin6_addr, host_address,
                   INET6_ADDRSTRLEN)) {
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }

    inet_address.family = IPFamily::IPv6;
  } else {
    ::freeaddrinfo(addr_info);
    throw std::runtime_error("Cannot parse host: Unknown family");
  }

  *inet_address.address = *addr_info->ai_addr;
  inet_address.host = host_address;
  ::freeaddrinfo(addr_info);

  return inet_address;
}

const sockaddr_in *InetAddress::AddressV4() const noexcept {
  if (family != IPFamily::IPv4) {
    return nullptr;
  }
  return (const sockaddr_in *)(address);
}

const sockaddr_in6 *InetAddress::AddressV6() const noexcept {
  if (family != IPFamily::IPv6) {
    return nullptr;
  }
  return (const sockaddr_in6 *)(address);
}

String InetAddress::ToString() const noexcept {
  return host + ":" + String::From(port);
}
}
}
