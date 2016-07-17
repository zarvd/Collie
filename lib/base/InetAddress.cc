#include "../../inc/base/InetAddress.h"
#include <netdb.h>
#include "../../inc/base/Logger.h"

namespace collie {

InetAddress::InetAddress(sockaddr *address) noexcept : InetAddress() {
  if (!address) return;

  this->address = address;
  char host[NI_MAXHOST];
  int ret = ::getnameinfo(address, sizeof(*address), host, NI_MAXHOST, nullptr,
                          0, NI_NUMERICHOST);

  this->host = host;
  if (address->sa_family == AF_INET) {
    this->family = IPFamily::IPv4;
    this->port = ::ntohs(((sockaddr_in *)address)->sin_port);
  } else {
    this->family = IPFamily::IPv6;
    this->port = ::ntohs(((sockaddr_in6 *)address)->sin6_port);
  }

  if (ret != 0) {
    LOG(WARN) << "Cannot parse address";
  }
}

InetAddress::InetAddress(const InetAddress &that) noexcept
    : host(that.host),
      port(that.port),
      family(that.family) {
  address = new sockaddr;
  *address = *that.address;
}

InetAddress &InetAddress::operator=(const InetAddress &that) noexcept {
  host = that.host;
  port = that.port;
  family = that.family;
  if (!address) address = new sockaddr;
  *address = *that.address;
  return *this;
}

InetAddress::InetAddress(InetAddress &&that) noexcept
    : host(std::move(that.host)),
      port(that.port),
      family(that.family) {
  address = that.address;
  that.address = nullptr;
}

InetAddress &InetAddress::operator=(InetAddress &&that) noexcept {
  host = that.host;
  port = that.port;
  family = that.family;
  if (address) delete address;
  address = that.address;
  that.address = nullptr;
  return *this;
}

InetAddress::~InetAddress() noexcept {
  if (address) delete address;
}

std::shared_ptr<InetAddress> InetAddress::GetInetAddress(const String &host,
                                                         const unsigned port) {
  // Gets address info
  addrinfo *addr_info;
  int err_code = ::getaddrinfo(host.RawData(), nullptr, nullptr, &addr_info);
  if (err_code != 0) {
    throw std::runtime_error(std::string("Cannot parse host: ") +
                             ::gai_strerror(err_code));
  }

  auto inet_address = std::make_shared<InetAddress>();
  inet_address->address = new sockaddr;
  inet_address->port = port;

  char host_address[INET6_ADDRSTRLEN];
  if (addr_info->ai_family == AF_INET) {
    // IPv4
    sockaddr_in *addr = ((sockaddr_in *)addr_info->ai_addr);
    addr->sin_port = ::htons(port);
    if (!inet_ntop(AF_INET, &addr->sin_addr, host_address, INET_ADDRSTRLEN)) {
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }

    inet_address->family = IPFamily::IPv4;
  } else if (addr_info->ai_family == AF_INET6) {
    // IPv6
    sockaddr_in6 *addr = ((sockaddr_in6 *)addr_info->ai_addr);
    addr->sin6_port = ::htons(port);
    if (!inet_ntop(AF_INET6, &addr->sin6_addr, host_address,
                   INET6_ADDRSTRLEN)) {
      throw std::runtime_error(std::string("Cannot parse host: ") +
                               ::strerror(errno));
    }

    inet_address->family = IPFamily::IPv6;
  } else {
    ::freeaddrinfo(addr_info);
    throw std::runtime_error("Cannot parse host: Unknown family");
  }

  *inet_address->address = *addr_info->ai_addr;
  inet_address->host = host_address;
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
