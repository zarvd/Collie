#include <sys/socket.h>
#include <netinet/in.h>
#include "../../include/udp/udp_socket.h"
#include "../../include/inet_address.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace udp {

UDPSocket::UDPSocket(std::shared_ptr<InetAddress> addr)
    : fd_(), address_(addr) {
  Log(TRACE) << "UDP Socket is constructing";
}

UDPSocket::~UDPSocket() { Log(TRACE) << "UDP Socket is destructing"; }

void UDPSocket::Listen() {
  REQUIRE(address_ && address_->ip_version() != IP::UNKNOWN);
  if (address_->ip_version() == IP::V4) {
    ListenV4();
  } else {
    ListenV6();
  }
}

void UDPSocket::ListenV4() {
  Log(DEBUG) << "UDP Socket is listening";
  fd_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  REQUIRE_SYS(fd_ != -1);
  struct sockaddr_in serv_address = address_->addr_v4();

  int ret = ::bind(fd_, (struct sockaddr *)&serv_address, sizeof(serv_address));
  REQUIRE_SYS(ret != -1);
}

void UDPSocket::ListenV6() {
  // TODO
  THROW_("TO BE CONTINUED");
}

void UDPSocket::Connect(IP ip_version) {
  REQUIRE(ip_version != IP::UNKNOWN);
  if (ip_version == IP::V4) {
    ConnectV4();
  } else {
    ConnectV6();
  }
}

void UDPSocket::ConnectV4() {
  Log(DEBUG) << "UDP Socket is connecting";
  fd_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  REQUIRE_SYS(fd_ != -1);
}

void UDPSocket::ConnectV6() {
  // TODO
  THROW_("TO BE CONTINUED");
}
}
}
