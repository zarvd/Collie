#include "../../include/network/Socket.hpp"
#include "../../include/network/SocketAddress.hpp"
#include "../../include/network/Network.hpp"


namespace Collie { namespace Network {

Socket::Socket() :
    type(Type::Client),
    fd(-1) {
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

}}
