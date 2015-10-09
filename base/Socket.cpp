#include "Socket.hpp"


namespace MiniHttp { namespace Base {

Socket::Socket(const unsigned& port) :
    fd(socket(port)),
    port(port) {
}

int Socket::socket(const unsigned& port) {
    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int socketFd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketFd < 0) {
        Log(ERROR) << "socket(): " << getErr();
    }

    int ret = bind(socketFd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if(ret < 0) {
        Log(ERROR) << "bind(): " << getErr();
    }
    return socketFd;
}

void Socket::listen() const {
    if(::listen(fd, SOMAXCONN) < 0) {
        Log(ERROR) << "listen(): " << getErr();
    }
}

int Socket::accept(std::shared_ptr<SocketAddress> addr) const {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    clientAddrLen = sizeof(clientAddr);

    int connFd;
    connFd = ::accept(fd, (struct sockaddr *)&clientAddr, &clientAddrLen);

    if(connFd < 0) {
        Log(ERROR) << "accept(): " << getErr();
    }
    *addr = clientAddr;

    return connFd;
}

void Socket::close() {
    ::close(fd);
}

}}
