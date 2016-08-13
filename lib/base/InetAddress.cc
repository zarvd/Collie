#include "../../inc/base/InetAddress.h"
#include <netdb.h>
#include <cstring>
#include "../../inc/base/Logger.h"

namespace collie {

InetAddress::InetAddress(const String &ip, const unsigned port,
                         const IPFamily family)
    : host(ip), port(port), family(family) {
  if (family == IPFamily::UNKNOWN)
    throw std::runtime_error("Unknown ip version");

  if (family == IPFamily::IPv4) {
    auto addr = new sockaddr_in;
    address.reset((sockaddr *)addr);

    addr->sin_port = ::htons(port);
    addr->sin_family = AF_INET;
    if (inet_pton(AF_INET, ip.RawData(), &addr->sin_addr) <= 0) {
      throw std::runtime_error(::strerror(errno));
    }
  } else {
    auto addr = new sockaddr_in6;
    address.reset((sockaddr *)addr);

    addr->sin6_port = ::htons(port);
    addr->sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, ip.RawData(), &addr->sin6_addr) <= 0) {
      throw std::runtime_error(::strerror(errno));
    }
  }
}

InetAddress::InetAddress(std::unique_ptr<sockaddr> addr, const IPFamily family)
    : family(family) {
  if (family == IPFamily::UNKNOWN)
    throw std::runtime_error("Unknown ip version");

  address = std::move(addr);
  if (!address) throw std::runtime_error("Nullable socket address");

  if (family == IPFamily::IPv4) {
    // For IPv4
    char host[INET_ADDRSTRLEN];
    auto ret = ::inet_ntop(AF_INET, &((sockaddr_in *)address.get())->sin_addr,
                           host, INET_ADDRSTRLEN);
    if (ret == nullptr) throw std::runtime_error("Unknown hostname");

    this->host = host;
    this->port = ::ntohs(((sockaddr_in *)address.get())->sin_port);
  } else {
    char host[INET6_ADDRSTRLEN];
    auto ret =
        ::inet_ntop(AF_INET6, &((sockaddr_in6 *)address.get())->sin6_addr, host,
                    INET6_ADDRSTRLEN);
    if (ret == nullptr) throw std::runtime_error("Unknown hostname");

    this->host = host;
    this->port = ::ntohs(((sockaddr_in6 *)address.get())->sin6_port);
  }
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
