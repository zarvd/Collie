#include "../../include/Global.hpp"
#include "../../include/udp/UDPSocket.hpp"
#include "../../include/SocketAddress.hpp"
#include <sys/socket.h>
#include <netinet/in.h>

namespace Collie {
namespace UDP {

UDPSocket::UDPSocket() : Socket() {
    Log(TRACE) << "UDP client Socket is constructing";
}

UDPSocket::UDPSocket(std::shared_ptr<SocketAddress> addr) : Socket(addr) {
    Log(TRACE) << "UDP server Socket is constructing";
}

UDPSocket::~UDPSocket() { Log(TRACE) << "UDP Socket is destructing"; }

void
UDPSocket::listen() {
    REQUIRE(localAddr && localAddr->getIPVersion() != IP::Unknown);
    if(localAddr->getIPVersion() == IP::V4) {
        listenV4();
    } else {
        listenV6();
    }
}

void
UDPSocket::listenV4() {
    Log(DEBUG) << "UDP Socket is listening";
    REQUIRE(type == Type::Server);
    fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    REQUIRE_SYS(fd != -1);
    struct sockaddr_in servAddr = localAddr->getAddrV4();

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
    REQUIRE(type == Type::Client);
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
