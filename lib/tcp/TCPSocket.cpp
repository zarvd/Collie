#include <netinet/in.h>
#include "../../include/tcp/TCPSocket.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/Global.hpp"

namespace Collie {
namespace TCP {

TCPSocket::TCPSocket() : Socket(), sendFlag(0), recvFlag(0) {
    Log(TRACE) << "TCPSocket client is constructing";
}

TCPSocket::TCPSocket(std::shared_ptr<SocketAddress> addr)
    : Socket(addr), sendFlag(0), recvFlag(0) {
    Log(TRACE) << "TCPSocket server is constructing";
}

TCPSocket::~TCPSocket() { Log(TRACE) << "TCPSocket is destructing"; }

void
TCPSocket::setSendFlag(const int flag) {
    sendFlag = flag;
}

void
TCPSocket::setRecvFlag(const int flag) {
    recvFlag = flag;
}

void
TCPSocket::listen() {
    if(localAddr->getIPVersion() == IP::V4) {
        listenV4();
    } else {
        listenV6();
    }
}

void
TCPSocket::listenV4() {
    REQUIRE(localAddr);

    struct sockaddr_in servAddr = localAddr->getAddrV4();

    fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        THROW_SYS;
    }

    int ret = bind(fd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if(ret < 0) {
        THROW_SYS;
    }
    Log(TRACE) << "Socket " << fd << " is binding";

    Log(TRACE) << "Socket is listening";

    if(::listen(fd, SOMAXCONN) < 0) {
        THROW_SYS;
    }
}

void
TCPSocket::listenV6() {
    // TODO
}

void
TCPSocket::connect(std::shared_ptr<SocketAddress> servAddr) {
    if(servAddr->getIPVersion() == IP::V4) {
        connectV4(servAddr);
    } else {
        connectV6(servAddr);
    }
}

void
TCPSocket::connectV4(std::shared_ptr<SocketAddress> servAddr) {
    struct sockaddr_in serv = servAddr->getAddrV4();
    fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        THROW_SYS;
    }
    int ret = ::connect(fd, (struct sockaddr *)&serv, sizeof(serv));
    if(ret == -1) {
        THROW_SYS;
    }
    struct sockaddr_in local;
    int addrLen = sizeof(local);
    ::getsockname(fd, (struct sockaddr *)&local, (socklen_t *)&addrLen);
    *localAddr = local;
}

void TCPSocket::connectV6(std::shared_ptr<SocketAddress>) {
    // TODO
}

// Thread safe
int
TCPSocket::accept(std::shared_ptr<SocketAddress> connAddr,
                  bool isNonBlock) const {
    return TCPSocket::accept(fd, connAddr, isNonBlock);
}

// Thread safe
int
TCPSocket::accept(const int fd, std::shared_ptr<SocketAddress> connAddr,
                  bool isNonBlock) {
    // IPv4
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    clientAddrLen = sizeof(clientAddr);

    int connFd;
    int flags = 0;
    if(isNonBlock) {
        Log(TRACE) << "Socket non blocking accept";
        flags = SOCK_NONBLOCK;
    }
    connFd =
        ::accept4(fd, (struct sockaddr *)&clientAddr, &clientAddrLen, SOCK_NONBLOCK);
    if(connFd > 2) {
        Log(TRACE) << "Socket accept connection " << connFd;
        *connAddr = clientAddr;
    } else {
        Log(DEBUG) << "Socket accept nothing";
    }

    return connFd;
}

std::string
TCPSocket::recv(const int connFd) {
    std::string content;
    Socket::recv(connFd, content, recvFlag);
    return content;
}

std::string
TCPSocket::recv() {
    std::string content;
    Socket::recv(content, recvFlag);
    return content;
}

void
TCPSocket::send(const std::string & msg) {
    Socket::send(msg, sendFlag);
}

void
TCPSocket::send(const int connFd, const std::string & msg) {
    Socket::send(connFd, msg, sendFlag);
}
}
}
