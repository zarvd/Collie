#include <netinet/in.h>
#include <unistd.h>
#include "../../include/tcp/TCPSocket.hpp"
#include "../../include/Socket.hpp"
#include "../../include/InetAddress.hpp"
#include "../../include/Global.hpp"
#include "../../include/utils/File.hpp"

namespace Collie {
namespace TCP {

TCPSocket::TCPSocket(SharedPtr<InetAddress> addr) noexcept : fd(-1),
                                                             state(State::Init),
                                                             address(addr) {
    Log(TRACE) << "TCPSocket is constructing";
    fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0) {
        Log(WARN) << "TCPSocket: " << getError();
    } else {
        state = State::Socket;
    }
}

// construct Accept connection socket
TCPSocket::TCPSocket(const int fd, SharedPtr<InetAddress> addr) noexcept
    : fd(fd),
      state(State::Accept),
      address(addr) {
    Log(TRACE) << "TCPSocket(Accept connection) is constructing";
}

// construct illegal socket
TCPSocket::TCPSocket() noexcept : fd(-1), state(State::IllegalAccept) {
    Log(TRACE) << "TCPSocket(IllegalAccept) is constructing";
}

TCPSocket::~TCPSocket() noexcept { Log(TRACE) << "TCPSocket is destructing"; }

void
TCPSocket::close() noexcept {
    Log(TRACE) << "Socket closed";
    if(state != State::Init && state != State::Close) {
        ::close(fd);
    }
}

bool
TCPSocket::bindAndListen() {
    REQUIRE(address);
    switch(address->getIPVersion()) {
    case IP::V4:
        return listenV4();
    default:
        return false;
    }
}

bool
TCPSocket::listenV4() {

    if(state == State::Socket) {
        struct sockaddr_in servAddr = address->getAddrV4();

        int ret = bind(fd, (struct sockaddr *)&servAddr, sizeof(servAddr));
        if(ret < 0) {
            Log(WARN) << "bind()" << getError();
        } else {
            state = State::Bind;
        }
    }
    if(state == State::Bind) {
        Log(TRACE) << "Socket " << fd << " is binding";

        Log(TRACE) << "Socket is listening";

        if(::listen(fd, SOMAXCONN) < 0) {
            Log(WARN) << getError();
        } else {
            state = State::Listen;
        }
    }
    if(state == State::Listen) {
        return true;
    } else {
        return false;
    }
}

bool
TCPSocket::connect(SharedPtr<InetAddress> servAddr) {
    REQUIRE(servAddr);
    switch(servAddr->getIPVersion()) {
    case IP::V4:
        return connectV4(servAddr);
    default:
        return false;
    }
}

bool
TCPSocket::connectV4(SharedPtr<InetAddress> servAddr) {
    struct sockaddr_in serv = servAddr->getAddrV4();
    if(state == State::Socket) {
        int ret = ::connect(fd, (struct sockaddr *)&serv, sizeof(serv));
        if(ret < 0) {
            Log(WARN) << "connect()" << getError();
        } else {
            state = State::Connect;

            // get local address
            struct sockaddr_in local;
            int addrLen = sizeof(local);
            ::getsockname(fd, (struct sockaddr *)&local, (socklen_t *)&addrLen);
            *address = local;
            return true;
        }
    }
    return false;
}

// Thread safe
SharedPtr<TCPSocket>
TCPSocket::accept(bool blocking) noexcept {
    REQUIRE(address);
    switch(address->getIPVersion()) {
    case IP::V4:
        return acceptV4(blocking);
    default:
        return getIllegalAcceptSocket();
    }
}

SharedPtr<TCPSocket>
TCPSocket::acceptV4(bool blocking) {
    if(state == State::Listen) {
        int flags = 0;
        if(blocking) flags = SOCK_NONBLOCK;

        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen;
        clientAddrLen = sizeof(clientAddr);

        const int ret =
            ::accept(fd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if(ret < 0) {
            Log(TRACE) << "accept(): " << getError();
            return getIllegalAcceptSocket();
        } else {
            Log(DEBUG) << "Socket accept " << ret;
            auto addr = MakeShared<InetAddress>(clientAddr);
            return getAcceptSocket(ret, addr);
        }
    }
    return getIllegalAcceptSocket();
}

ssize_t
TCPSocket::recv(String & content, const int flags) {
    REQUIRE(state == State::Connect || state == State::Accept);
    return Socket::recv(shared_from_this(), content, flags);
}

ssize_t
TCPSocket::send(const String & content, const int flags) {
    REQUIRE(state == State::Connect || state == State::Accept);
    return Socket::send(shared_from_this(), content, flags);
}

bool
TCPSocket::sendFile(const Utils::File & file) {
    REQUIRE(state == State::Connect || state == State::Accept);
    return Socket::sendFile(shared_from_this(), file);
}

bool
TCPSocket::recvFile(Utils::File & file, const size_t recvSize) {
    REQUIRE(state == State::Connect || state == State::Accept);
    return Socket::recvFile(shared_from_this(), file, recvSize);
}

SharedPtr<TCPSocket>
TCPSocket::getAcceptSocket(const int fd, SharedPtr<InetAddress> addr) noexcept {
    REQUIRE(fd > 0);
    return SharedPtr<TCPSocket>(new TCPSocket(fd, addr));
}

SharedPtr<TCPSocket>
TCPSocket::getIllegalAcceptSocket() noexcept {
    static const auto illegalSocket = SharedPtr<TCPSocket>(new TCPSocket());
    return illegalSocket;
}
}
}
