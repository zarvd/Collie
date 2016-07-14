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
class InetAddress : public util::NonCopyable {
 public:
  using Host = std::string;
  using Port = unsigned;
  using Address = sockaddr*;
  using AddressV4 = sockaddr_in*;
  using AddressV6 = sockaddr_in6*;

  InetAddress(const Host&, const Port&, const Address,
              const IPFamily&) noexcept;
  ~InetAddress() noexcept;

  IPFamily GetIPFamily() const { return ip_family; }
  Host GetHost() const noexcept { return host; }
  Port GetPort() const noexcept { return port; }
  Address GetAddress() const noexcept { return address; }
  AddressV4 GetIPv4Address() const throw(std::runtime_error);
  AddressV6 GetIPv6Address() const throw(std::runtime_error);

  static std::shared_ptr<InetAddress> GetInetAddress(
      const Host&, const Port) throw(std::runtime_error);

 private:
  Host host;
  Port port;
  Address address;
  IPFamily ip_family;
};
}

#endif /* COLLIE_INET_ADDRESS_H_ */
