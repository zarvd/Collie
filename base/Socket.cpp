#include "Socket.hpp"


namespace MiniHttp { namespace Base {

Socket::Socket(const unsigned& port) :
    fd(socket(port)),
    port(port) {
    Log(TRACE) << "Socket constructing";
}

Socket::~Socket() {
    Log(TRACE) << "Socket destructing";
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
    Log(TRACE) << "Socket " << socketFd << " bound";
    return socketFd;
}

void Socket::listen() const {
    Log(TRACE) << "Socket listening";
    if(::listen(fd, SOMAXCONN) < 0) {
        Log(ERROR) << "listen(): " << getErr();
    }
}

int Socket::accept(std::shared_ptr<SocketAddress> addr) const {
    return Socket::accept(fd, addr);
}

int Socket::accept(const int & fd, std::shared_ptr<SocketAddress> addr) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    clientAddrLen = sizeof(clientAddr);

    int connFd;
    connFd = ::accept(fd, (struct sockaddr *)&clientAddr, &clientAddrLen);

    Log(TRACE) << "Socket accept connection " << connFd;
    if(connFd < 0) {
        Log(ERROR) << "accept(): " << getErr();
    }
    *addr = clientAddr;

    return connFd;
}

void Socket::close() {
    Log(TRACE) << "Socket closed";
    ::close(fd);
}

std::string Socket::recv(const int & fd) {
    const unsigned msgLength = 8192;  // FIXME
    char msg[msgLength];
    ::recv(fd, msg, msgLength, 0);
    Log(TRACE) << "Socket received msg";
    return msg;
}

void Socket::send(const int & fd, const std::string & msg) {
    char page[msg.length() + 1];
    std::strcpy(page, msg.c_str());
    ::send(fd, page, sizeof(page), 0);
    Log(TRACE) << "Socket send msg";
}

}}
