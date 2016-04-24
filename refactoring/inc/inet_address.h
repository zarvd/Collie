#ifndef COLLIE_INET_ADDRESS_H_
#define COLLIE_INET_ADDRESS_H_

#include <arpa/inet.h>
#include <memory>
#include <string>
#include "util/noncopyable.h"

namespace collie {

enum class IPVersion { IPV4, IPV6, UNKNOWN };

// 网络地址抽象类
class InetAddress : public NonCopyable {
 public:
  using Host = std::string;
  using Port = unsigned;
  using Address = sockaddr*;

  InetAddress() {}

  virtual ~InetAddress() = 0;

  virtual IPVersion ip_version() const = 0;
  Host host() const { return host_; }
  Port port() const { return port_; }
  Address address() const { return address_; }

  static std::shared_ptr<InetAddress> GetInetAddress(const Host& host,
                                                     const Port port);

 protected:
  Host host_;
  Port port_;
  Address address_;
};

// IPv4地址
class IPV4Address : public InetAddress {
 public:
  IPV4Address() {}
  ~IPV4Address() noexcept override {}

  IPVersion ip_version() const override { return IPVersion::IPV4; }
};

// IPv6地址
class IPV6Address : public InetAddress {
 public:
  IPV6Address() {}
  ~IPV6Address() noexcept override {}

  IPVersion ip_version() const override { return IPVersion::IPV4; }
};
}

#endif /* COLLIE_INET_ADDRESS_H_ */
