#ifndef COLLIE_INET_ADDRESS_H_
#define COLLIE_INET_ADDRESS_H_

#include <string>
#include <memory>
#include <arpa/inet.h>

namespace collie {

enum class IP { V4, V6, UNKNOWN };

class InetAddress {
 public:
  using AddrV4 = struct sockaddr_in;
  using AddrV6 = struct sockaddr_in6;

  InetAddress();
  explicit InetAddress(const AddrV4 &);
  explicit InetAddress(const AddrV6 &);
  InetAddress(const InetAddress &);
  ~InetAddress();

  InetAddress &operator=(const InetAddress &);
  InetAddress &operator=(const AddrV4 &);
  InetAddress &operator=(const AddrV6 &);

  std::string ip() const { return ip_; }
  unsigned port() const { return port_; }
  IP ip_version() const { return ip_version_; }
  AddrV4 addr_v4() const { return addr_v4_; }
  AddrV6 addr_v6() const { return addr_v6_; }

  // TODO detect host's type(ip, domain) and ip version
  // then create sockaddr_in or sockaddr_in6
  // Note: it will block
  static std::shared_ptr<InetAddress> GetInetAddress(const std::string &host,
                                                     const unsigned port);

 private:
  std::string ip_;
  unsigned port_;
  IP ip_version_;
  AddrV4 addr_v4_;
  AddrV6 addr_v6_;
};
}

#endif /* COLLIE_INET_ADDRESS_H_ */
