#ifndef COLLIE_INET_ADDRESS_H_
#define COLLIE_INET_ADDRESS_H_

#include <arpa/inet.h>
#include <memory>
#include <string>
#include "util/noncopyable.h"

namespace collie {

enum class IPFamily { IPv4, IPv6, UNKNOWN };

inline std::string to_string(const IPFamily& family) {
  switch (family) {
    case IPFamily::IPv4:
      return "IPv4";
    case IPFamily::IPv6:
      return "IPv6";
    case IPFamily::UNKNOWN:
      return "Unknown";
  }
}

// Abstract class
class InetAddress : public NonCopyable {
 public:
  using Host = std::string;
  using Port = unsigned;
  using Address = sockaddr*;
  using AddressV4 = sockaddr_in*;
  using AddressV6 = sockaddr_in6*;

  InetAddress() noexcept : address_() {}

  virtual ~InetAddress() noexcept = 0;

  virtual IPFamily ip_version() const = 0;
  Host host() const noexcept { return host_; }
  Port port() const noexcept { return port_; }
  Address address() const noexcept { return address_; }
  AddressV4 GetIPv4Address() const throw(std::runtime_error);
  AddressV6 GetIPv6Address() const throw(std::runtime_error);

  static std::shared_ptr<InetAddress> GetInetAddress(
      const Host&, const Port) throw(std::runtime_error);

 protected:
  Host host_;
  Port port_;
  Address address_;
};

// IPv4 address
class IPv4Address : public InetAddress {
 public:
  IPv4Address() noexcept {}
  ~IPv4Address() noexcept override {}

  IPFamily ip_version() const override { return IPFamily::IPv4; }
};

// IPv6 address
class IPv6Address : public InetAddress {
 public:
  IPv6Address() noexcept {}
  ~IPv6Address() noexcept override {}

  IPFamily ip_version() const override { return IPFamily::IPv6; }
};
}

#endif /* COLLIE_INET_ADDRESS_H_ */
