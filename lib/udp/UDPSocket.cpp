#include "../../include/Global.hpp"
#include "../../include/udp/UDPSocket.hpp"
#include "../../include/InetAddress.hpp"
#include <sys/socket.h>
#include <netinet/in.h>

namespace Collie {
namespace UDP {

UDPSocket::UDPSocket(SharedPtr<InetAddress> addr) : address(addr) {
    Log(TRACE) << "UDP Socket is constructing";
}

UDPSocket::~UDPSocket() { Log(TRACE) << "UDP Socket is destructing"; }

void
UDPSocket::listen() {
    REQUIRE(address && address->getIPVersion() != IP::Unknown);
    if(address->getIPVersion() == IP::V4) {
        listenV4();
    } else {
        listenV6();
    }
}

void
UDPSocket::listenV4() {
    Log(DEBUG) << "UDP Socket is listening";
    fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    REQUIRE_SYS(fd != -1);
    struct sockaddr_in servAddr = address->getAddrV4();

    int ret = ::bind(fd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    REQUIRE_SYS(ret != -1);
}

void
UDPSocket::listenV6() {
    // TODO
    THROW_("TO BE CONTINUED");
}

void
UDPSocket::connect(IP ipVersion) {
    REQUIRE(ipVersion != IP::Unknown);
    if(ipVersion == IP::V4) {
        connectV4();
    } else {
        connectV6();
    }
}

void
UDPSocket::connectV4() {
    Log(DEBUG) << "UDP Socket is connecting";
    fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    REQUIRE_SYS(fd != -1);
}

void
UDPSocket::connectV6() {
    // TODO
    THROW_("TO BE CONTINUED");
}
}
}
