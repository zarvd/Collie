#ifndef COLLIE_INET_ADDRESS_H_
#define COLLIE_INET_ADDRESS_H_

#include <arpa/inet.h>
#include "Serializable.h"
#include "String.h"

namespace collie {
namespace base {

enum class IPFamily { IPv4, IPv6, UNKNOWN };

class InetAddress final : public Serializable {
 public:
  ~InetAddress() noexcept;
  InetAddress(const InetAddress&) noexcept;
  InetAddress& operator=(const InetAddress&) noexcept;
  InetAddress(InetAddress&&) noexcept;
  InetAddress& operator=(InetAddress&&) noexcept;

  IPFamily Family() const { return family; }
  String Host() const noexcept { return host; }
  unsigned Port() const noexcept { return port; }
  const sockaddr* Address() const noexcept { return address; }
  const sockaddr_in* AddressV4() const noexcept;
  const sockaddr_in6* AddressV6() const noexcept;
  String ToString() const noexcept override;

  static InetAddress GetInetAddress(const String& host, const unsigned port);
  bool IsLoopback() const noexcept;

 private:
  InetAddress() : address(nullptr), family(IPFamily::UNKNOWN) {}

  String host;
  unsigned port;
  sockaddr* address;
  IPFamily family;
};
}
}

#endif /* COLLIE_INET_ADDRESS_H_ */
