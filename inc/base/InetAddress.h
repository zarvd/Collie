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
  InetAddress() noexcept : port(0),
                           address(nullptr),
                           family(IPFamily::UNKNOWN) {}

  explicit InetAddress(std::unique_ptr<sockaddr>);

  ~InetAddress();
  InetAddress(const InetAddress&) noexcept;
  InetAddress& operator=(const InetAddress&) noexcept;
  InetAddress(InetAddress&&) noexcept;
  InetAddress& operator=(InetAddress&&) noexcept;

  IPFamily Family() const { return family; }
  String Host() const noexcept { return host; }
  unsigned Port() const noexcept { return port; }
  const sockaddr* Address() const noexcept { return address.get(); }
  const sockaddr_in* AddressV4() const noexcept;
  const sockaddr_in6* AddressV6() const noexcept;
  String ToString() const noexcept override;

  // Host could be ip address or domain URL
  static std::shared_ptr<InetAddress> GetInetAddress(const String& host,
                                                     const unsigned port);
  bool IsLoopback() const noexcept;

 private:
  String host;
  unsigned port;
  std::unique_ptr<sockaddr> address;
  IPFamily family;
};
}

#endif /* COLLIE_BASE_INET_ADDRESS_H_ */
