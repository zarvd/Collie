#include <netinet/in.h>
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/Global.hpp"

namespace Collie {
namespace Tcp {

TcpSocket::TcpSocket() : Socket(), sendFlag(0), recvFlag(0) {
    Log(TRACE) << "TcpSocket client is constructing";
}

TcpSocket::TcpSocket(std::shared_ptr<SocketAddress> addr)
    : Socket(addr), sendFlag(0), recvFlag(0) {
    Log(TRACE) << "TcpSocket server is constructing";
}

TcpSocket::~TcpSocket() { Log(TRACE) << "TcpSocket is destructing"; }

void
TcpSocket::setSendFlag(const int flag) {
    sendFlag = flag;
}

void
TcpSocket::setRecvFlag(const int flag) {
    recvFlag = flag;
}

void
TcpSocket::listen() {
    if(localAddr->getIPVersion() == IP::V4) {
        listenV4();
    } else {
        listenV6();
    }
}

void
TcpSocket::listenV4() {
    if(!localAddr) {
        Log(ERROR) << "local address is null";
        THROW_INVALID_ARGUMENT;
    }
    struct sockaddr_in servAddr = localAddr->getAddrV4();

    fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        Log(ERROR) << "socket(): " << Exception::getErr();
        THROW_SYS;
    }

    int ret = bind(fd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if(ret < 0) {
        Log(ERROR) << "bind(): " << Exception::getErr();
        THROW_SYS;
    }
    Log(TRACE) << "Socket " << fd << " is binding";

    Log(TRACE) << "Socket is listening";

    if(::listen(fd, SOMAXCONN) < 0) {
        Log(ERROR) << "listen(): " << Exception::getErr();
        THROW_SYS;
    }
}

void
TcpSocket::listenV6() {
    // TODO
}

void
TcpSocket::connect(std::shared_ptr<SocketAddress> servAddr) {
    if(servAddr->getIPVersion() == IP::V4) {
        connectV4(servAddr);
    } else {
        connectV6(servAddr);
    }
}

void
TcpSocket::connectV4(std::shared_ptr<SocketAddress> servAddr) {
    struct sockaddr_in serv = servAddr->getAddrV4();
    fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        Log(ERROR) << "socket(): " << Exception::getErr();
        THROW_SYS;
    }
    int ret = ::connect(fd, (struct sockaddr *)&serv, sizeof(serv));
    if(ret == -1) {
        Log(ERROR) << "connect(): " << Exception::getErr();
        THROW_SYS;
    }
    struct sockaddr_in local;
    int addrLen = sizeof(local);
    ::getsockname(fd, (struct sockaddr *)&local, (socklen_t *)&addrLen);
    *localAddr = local;
}

void TcpSocket::connectV6(std::shared_ptr<SocketAddress>) {
    // TODO
}

int
TcpSocket::accept(std::shared_ptr<SocketAddress> connAddr) const {
    return TcpSocket::accept(fd, connAddr);
}

int
TcpSocket::accept(const int fd, std::shared_ptr<SocketAddress> connAddr) {
    // IPv4
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    clientAddrLen = sizeof(clientAddr);

    int connFd;
    connFd = ::accept(fd, (struct sockaddr *)&clientAddr, &clientAddrLen);

    Log(TRACE) << "Socket accept connection " << connFd;
    if(connFd < 0) {
        Log(ERROR) << "accept(): " << Exception::getErr();
        THROW_SYS;
    }
    *connAddr = clientAddr;

    return connFd;
}

std::string
TcpSocket::recv(const int connFd) {
    return recv(connFd, recvFlag);
}

std::string
TcpSocket::recv(const int connFd, const int recvFlag) {
    if(connFd < 2) {
        Log(WARN) << "Illegal connection fd " << connFd;
    }
    constexpr size_t msgLength = 8192; // FIXME
    char msg[msgLength];
    ::recv(connFd, msg, msgLength, recvFlag);
    Log(TRACE) << "Socket received msg";
    return msg;
}

void
TcpSocket::send(const int connFd, const std::string & msg) {
    send(connFd, msg, sendFlag);
}

void
TcpSocket::send(const int connFd, const std::string & msg, const int sendFlag) {
    char content[msg.length() + 1];
    std::strcpy(content, msg.c_str());
    ::send(connFd, content, sizeof(content), sendFlag);
    Log(TRACE) << "Socket send msg";
}
}
}
