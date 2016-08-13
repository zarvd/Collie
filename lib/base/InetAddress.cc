#include "../../inc/base/InetAddress.h"
#include <netdb.h>
#include <cstring>
#include "../../inc/base/Logger.h"

namespace collie {

InetAddress::InetAddress(std::unique_ptr<sockaddr> addr) : InetAddress() {
  address = std::move(addr);
  if (!address) throw std::runtime_error("Nullable socket address");

  // For IPv4
  char host[200];
  auto ret = ::inet_ntop(AF_INET, address.get(), host, sizeof(host));
  if (ret == nullptr) throw std::runtime_error("Unknown hostname");

  this->host = host;
  family = IPFamily::IPv4;
  port = ::ntohs(((sockaddr_in *)address.get())->sin_port);

  // FIXME Supports IPv6
  // if (address->sa_family == AF_INET) {
  // } else {
  //   this->family = IPFamily::IPv6;
  //   this->port = ::ntohs(((sockaddr_in6 *)address.get())->sin6_port);
  // }
}

InetAddress::InetAddress(const InetAddress &that) noexcept
    : host(that.host),
      port(that.port),
      address(std::make_unique<sockaddr>(*that.address)),
      family(that.family) {}

InetAddress &InetAddress::operator=(const InetAddress &that) noexcept {
  host = that.host;
  port = that.port;
  family = that.family;
  address = std::make_unique<sockaddr>(*that.address);
  return *this;
}

InetAddress::InetAddress(InetAddress &&that) noexcept
    : host(std::move(that.host)),
      port(that.port),
      address(std::move(that.address)),
      family(that.family) {}

InetAddress &InetAddress::operator=(InetAddress &&that) noexcept {
  host = that.host;
  port = that.port;
  family = that.family;
  address = std::move(that.address);
  that.address = nullptr;
  return *this;
}

InetAddress::~InetAddress() {}

std::shared_ptr<InetAddress> InetAddress::GetInetAddress(const String &host,
                                                         const unsigned port) {
  // Gets address info
  addrinfo *addr_info;
  int ret = ::getaddrinfo(host.RawData(), nullptr, nullptr, &addr_info);
  throw std::runtime_error(::gai_strerror(ret));

  auto inet_address = std::make_shared<InetAddress>();
  inet_address->address = std::make_unique<sockaddr>();
  inet_address->port = port;

  char host_address[INET6_ADDRSTRLEN];  // Sets the max length
  if (addr_info->ai_family == AF_INET) {
    // IPv4
    sockaddr_in *addr = ((sockaddr_in *)addr_info->ai_addr);
    addr->sin_port = ::htons(port);
    if (!inet_ntop(AF_INET, &addr->sin_addr, host_address, INET_ADDRSTRLEN))
      throw std::runtime_error(::strerror(errno));

    inet_address->family = IPFamily::IPv4;
  } else if (addr_info->ai_family == AF_INET6) {
    // IPv6
    sockaddr_in6 *addr = ((sockaddr_in6 *)addr_info->ai_addr);
    addr->sin6_port = ::htons(port);
    if (!inet_ntop(AF_INET6, &addr->sin6_addr, host_address, INET6_ADDRSTRLEN))
      throw std::runtime_error(::strerror(errno));

    inet_address->family = IPFamily::IPv6;
  } else {
    // Unknown IP version
    ::freeaddrinfo(addr_info);
    throw std::runtime_error("Cannot parse host: Unknown family");
  }

  // Copy the struct instead of the pointer
  *inet_address->address = *addr_info->ai_addr;
  inet_address->host = host_address;
  ::freeaddrinfo(addr_info);

  return inet_address;
}

const sockaddr_in *InetAddress::AddressV4() const noexcept {
  if (family != IPFamily::IPv4) return nullptr;
  return (const sockaddr_in *)(address.get());
}

const sockaddr_in6 *InetAddress::AddressV6() const noexcept {
  if (family != IPFamily::IPv6) return nullptr;
  return (const sockaddr_in6 *)(address.get());
}

String InetAddress::ToString() const noexcept {
  return host + ":" + String::From(port);
}
}
