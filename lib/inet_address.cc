#include <netdb.h>
#include "../include/inet_address.h"
#include "../include/logging.h"

namespace collie {

InetAddress::InetAddress() : ip_(), port_(), ip_version_(IP::UNKNOWN) {}
InetAddress::~InetAddress() {}

InetAddress::InetAddress(const AddrV4 &addr)
    : port_(::ntohs(addr.sin_port)), ip_version_(IP::V4), addr_v4_(addr) {
  char ipv4[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(addr_v4_.sin_addr), ipv4, INET_ADDRSTRLEN);
}

InetAddress::InetAddress(const AddrV6 &) : port_(), ip_version_(IP::UNKNOWN) {
  // TODO
}

InetAddress &InetAddress::operator=(const InetAddress &that) {
  ip_ = that.ip_;
  port_ = that.port_;
  ip_version_ = that.ip_version_;
  addr_v4_ = that.addr_v4_;
  addr_v6_ = that.addr_v6_;
  return *this;
}

InetAddress &InetAddress::operator=(const AddrV4 &addr) {
  ip_version_ = IP::V4;
  addr_v4_ = addr;
  char ipv4[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(addr_v4_.sin_addr), ipv4, INET_ADDRSTRLEN);
  ip_ = ipv4;
  port_ = ::ntohs(addr.sin_port);
  return *this;
}

InetAddress &InetAddress::operator=(const AddrV6 &) {
  // TODO
  return *this;
}

std::shared_ptr<InetAddress> InetAddress::GetInetAddress(
    const std::string &host, const unsigned port) {
  auto addr = std::make_shared<InetAddress>();

  struct hostent *hostStruct = ::gethostbyname(host.c_str());
  CHECK_NE(hostStruct, nullptr) << hstrerror(h_errno);

  switch (hostStruct->h_addrtype) {
    case AF_INET:
      // IPv4
      char ipv4[INET_ADDRSTRLEN];
      if (::inet_ntop(AF_INET, *(hostStruct->h_addr_list), ipv4,
                      INET_ADDRSTRLEN) == NULL) {
        THROW_SYS;
      }
      addr->ip_ = ipv4;
      addr->port_ = port;
      addr->ip_version_ = IP::V4;
      addr->addr_v4_.sin_family = AF_INET;
      addr->addr_v4_.sin_port = ::htons(port);
      if (::inet_pton(AF_INET, addr->ip_.c_str(), &(addr->addr_v4_.sin_addr)) !=
          1) {
        THROW_SYS;
      }
      break;
    case AF_INET6:
      // IPv6
      char ipv6[INET6_ADDRSTRLEN];
      if (::inet_ntop(AF_INET6, *(hostStruct->h_addr_list), ipv6,
                      INET6_ADDRSTRLEN) == NULL) {
        THROW_SYS;
      }
      addr->ip_ = ipv6;
      addr->port_ = port;
      addr->ip_version_ = IP::V6;
      addr->addr_v6_.sin6_family = AF_INET6;
      addr->addr_v6_.sin6_port = ::htons(port);
      if (::inet_pton(AF_INET6, addr->ip_.c_str(),
                      &(addr->addr_v6_.sin6_addr)) != 1) {
        THROW_SYS;
      }
      break;
    default:
      break;
  }

  return addr;
}
}
