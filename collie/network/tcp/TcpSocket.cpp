#include <netinet/in.h>
#include "TcpSocket.hpp"
#include "../SocketAddress.hpp"
#include "../Network.hpp"


namespace Collie { namespace Network { namespace Tcp {

TcpSocket::TcpSocket(std::shared_ptr<SocketAddress> addr) : Socket(addr) {
    Log(TRACE) << "TcpSocket is constructing";
}

TcpSocket::~TcpSocket() {
    Log(TRACE) << "TcpSocket is destructing";
}

void TcpSocket::listen() {
    if(addr->getIPVersion() == IP::V4) {
        listenV4();
    } else {
        listenV6();
    }
}

void TcpSocket::listenV4() {
    struct sockaddr_in servAddr = addr->getAddrV4();

    // IPv4
    // servAddr.sin_family = AF_INET;
    // servAddr.sin_port = htons(port);
    // servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // FIXME

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

    Log(TRACE) << "Socket listening";

    if(::listen(fd, SOMAXCONN) < 0) {
        Log(ERROR) << "listen(): " << Exception::getErr();
        THROW_SYS;
    }
}

int TcpSocket::accept(std::shared_ptr<SocketAddress> connAddr) const {
    return TcpSocket::accept(fd, connAddr);
}

int TcpSocket::accept(const int & fd, std::shared_ptr<SocketAddress> connAddr) {
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

std::string TcpSocket::recv(const int & connFd) {
    const unsigned msgLength = 8192;  // FIXME
    char msg[msgLength];
    ::recv(connFd, msg, msgLength, 0);  // FIXME
    Log(TRACE) << "Socket received msg";
    return msg;
}

void TcpSocket::send(const int & connFd, const std::string & msg) {
    char content[msg.length() + 1];
    std::strcpy(content, msg.c_str());
    ::send(connFd, content, sizeof(content), 0);  // FIXME
    Log(TRACE) << "Socket send msg";
}

}}}
