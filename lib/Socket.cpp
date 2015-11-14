#include "../include/Global.hpp"
#include "../include/Socket.hpp"
#include "../include/SocketAddress.hpp"
#include <unistd.h>
#include <fcntl.h>

namespace Collie {

Socket::Socket() : type(Type::Client), fd(-1), localAddr(new SocketAddress) {
    Log(TRACE) << "Client socket constructing";
}

Socket::Socket(std::shared_ptr<SocketAddress> addr)
    : type(Type::Server), fd(-1), localAddr(addr) {
    Log(TRACE) << "Server socket constructing";
}

Socket::~Socket() { Log(TRACE) << "Socket destructing"; }

void
Socket::close() {
    Log(TRACE) << "Socket closed";
    ::close(fd);
}

bool
Socket::setFdNonBlocking(int fd) {
    auto flags = ::fcntl(fd, F_GETFL, 0);
    if(flags == -1) return false;
    flags |= O_NONBLOCK;
    if(::fcntl(fd, F_SETFL, flags) == -1) return false;
    return true;
}

ssize_t
Socket::send(const std::string & content, const int flag) {
    return send(fd, content, flag);
}

ssize_t
Socket::recv(std::string & content, const int flag) {
    return recv(fd, content, flag);
}

ssize_t
Socket::sendTo(const std::string & content,
               std::shared_ptr<SocketAddress> remoteAddr, const int flag) {
    return sendTo(fd, content, remoteAddr, flag);
}

ssize_t
Socket::recvFrom(std::string & content,
                 const std::shared_ptr<SocketAddress> & remoteAddr,
                 const int flag) {
    return recvFrom(fd, content, remoteAddr, flag);
}

ssize_t
Socket::send(const int socketFd, const std::string & content, const int flag) {
    char contentChars[content.length() + 1];
    std::strcpy(contentChars, content.c_str());
    Log(TRACE) << "Socket is sending " << contentChars;
    const ssize_t size =
        ::send(socketFd, contentChars, sizeof(contentChars), flag);
    if(size <= 0) {
        Log(DEBUG) << "Socket send nothing";
    } else {
        Log(TRACE) << "Socket send msg";
    }
    return size;
}

ssize_t
Socket::recv(const int socketFd, std::string & content, const int flag) {
    if(socketFd < 2) {
        Log(WARN) << "Illegal socket fd " << socketFd;
    }
    constexpr size_t msgLength = 8192; // FIXME
    char msg[msgLength];
    const ssize_t size = ::recv(socketFd, msg, msgLength, flag);
    if(size <= 0) {
        Log(DEBUG) << "Socket received nothing";
        content = "";
    } else {
        Log(TRACE) << "Socket received msg";
        content = msg;
    }

    return size;
}

ssize_t
Socket::sendTo(const int socketFd, const std::string & content,
               std::shared_ptr<SocketAddress> remoteAddr, const int flag) {
    char contentC[content.length() + 1];
    std::strcpy(contentC, content.c_str());
    struct sockaddr_in addr = remoteAddr->getAddrV4();
    const ssize_t size =
        ::sendto(socketFd, contentC, sizeof(contentC), flag,
                 (struct sockaddr *)&addr, sizeof(remoteAddr->getAddrV4()));
    if(size < 0) {
        THROW_SYS_(std::to_string(addr.sin_family));
    } else if(size == 0) {
        Log(DEBUG) << "Socket send nothing";
    } else {
        Log(TRACE) << "Socket send msg";
    }
    return size;
}

ssize_t
Socket::recvFrom(const int socketFd, std::string & content,
                 const std::shared_ptr<SocketAddress> & remoteAddr,
                 const int flag) {
    char buffer[8192];
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    const ssize_t size = ::recvfrom(socketFd, buffer, sizeof(buffer), flag,
                                    (struct sockaddr *)&addr, &addrLen);
    if(size < 0) {
        THROW_SYS;
    } else if(size == 0) {
        Log(DEBUG) << "Socket received nothing";
        content = "";
    } else {
        Log(TRACE) << "Socket received msg";
        content = buffer;
        (*remoteAddr) = addr;
    }
    return size;
}
}
