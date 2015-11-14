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
    REQUIRE(localAddr);
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
    int fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    REQUIRE(fd != -1);
    struct sockaddr_in servAddr = localAddr->getAddrV4();

    int ret = ::bind(fd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    REQUIRE(ret != -1);
}
}
}
