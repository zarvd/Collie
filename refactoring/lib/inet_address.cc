#include <netdb.h>
#include "../inc/inet_address.h"

namespace collie {

InetAddress::~InetAddress() {}

std::shared_ptr<InetAddress> InetAddress::GetInetAddress(const Host &host,
                                                         const Port port) {
  auto hostInfo = ::gethostbyname(host.c_str());
  if (!hostInfo) {
    // 无法获取host信息
    return nullptr;
  }
  if (hostInfo->h_addrtype == AF_INET) {
    // host类型为IPv4
    char host_address[INET_ADDRSTRLEN];
    // 将host中的第一个地址结构转换成表达式字符串
    if (!::inet_ntop(AF_INET, *(hostInfo->h_addr_list), host_address,
                     INET_ADDRSTRLEN)) {
      // 无法转换
      return nullptr;
    }

    auto inet_address = std::make_shared<IPV4Address>();
    inet_address->host_ = host_address;
    inet_address->port_ = port;

    sockaddr_in *addr = new sockaddr_in;
    addr->sin_family = AF_INET;
    addr->sin_port = ::htons(port);
    if (::inet_pton(AF_INET, host_address, &(addr->sin_addr)) != 1) {
      return nullptr;
    }
    inet_address->address_ = Address(addr);
    return inet_address;
  } else if (hostInfo->h_addrtype == AF_INET6) {
    // host类型为IPv6
    char host_address[INET6_ADDRSTRLEN];
    if (!::inet_ntop(AF_INET6, *(hostInfo->h_addr_list), host_address,
                     INET6_ADDRSTRLEN)) {
      return nullptr;
    }
    auto inet_address = std::make_shared<IPV6Address>();
    inet_address->host_ = host_address;
    inet_address->port_ = port;

    sockaddr_in6 *addr = new sockaddr_in6;
    addr->sin6_family = AF_INET;
    addr->sin6_port = ::htons(port);
    if (::inet_pton(AF_INET6, host_address, &(addr->sin6_addr)) != 1) {
      return nullptr;
    }
    inet_address->address_ = Address(addr);
    return inet_address;
  } else {
    return nullptr;
  }
}
}
