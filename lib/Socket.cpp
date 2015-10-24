#include "../include/Global.hpp"
#include "../include/Socket.hpp"
#include "../include/SocketAddress.hpp"


namespace Collie {

Socket::Socket() :
    type(Type::Client),
    fd(-1),
    localAddr(new SocketAddress) {
    Log(TRACE) << "Client socket constructing";
}

Socket::Socket(std::shared_ptr<SocketAddress> addr) :
    type(Type::Server),
    fd(-1),
    localAddr(addr) {
    Log(TRACE) << "Server socket constructing";
}

Socket::~Socket() {
    Log(TRACE) << "Socket destructing";
}

void Socket::close() {
    Log(TRACE) << "Socket closed";
    ::close(fd);
}

}
