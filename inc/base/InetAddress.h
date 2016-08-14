#ifndef COLLIE_BASE_INET_ADDRESS_H_
#define COLLIE_BASE_INET_ADDRESS_H_

#include <arpa/inet.h>
#include <memory>
#include "Serializable.h"
#include "String.h"

namespace collie {

enum class IPFamily { IPv4, IPv6, UNKNOWN };

class InetAddress final : public Serializable {
 public:
  InetAddress(const std::string& ip, const unsigned port,
              const IPFamily family = IPFamily::IPv4);

  explicit InetAddress(std::unique_ptr<sockaddr>,
                       const IPFamily family = IPFamily::IPv4);

  ~InetAddress();
  InetAddress(const InetAddress&) noexcept;
  InetAddress& operator=(const InetAddress&) noexcept;
  InetAddress(InetAddress&&) noexcept;
  InetAddress& operator=(InetAddress&&) noexcept;

  IPFamily Family() const { return family; }
  std::string Host() const noexcept { return host; }
  unsigned Port() const noexcept { return port; }
  const sockaddr* Address() const noexcept { return address.get(); }
  const sockaddr_in* AddressV4() const noexcept;
  const sockaddr_in6* AddressV6() const noexcept;
  std::string ToString() const noexcept override;

  bool IsLoopback() const noexcept;

 private:
  std::string host;
  unsigned port;
  std::unique_ptr<sockaddr> address;
  IPFamily family;
};
}

#endif /* COLLIE_BASE_INET_ADDRESS_H_ */
