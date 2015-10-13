#include "Socket.hpp"
#include "SocketAddress.hpp"
#include "Network.hpp"


namespace Collie { namespace Network {

Socket::Socket(const IP & ip, const unsigned& port, std::shared_ptr<SocketAddress> addr) :
    ipVersion(ip),
    fd(0),
    port(port),
    addr(addr) {
    Log(TRACE) << "Socket constructing";
    if(addr->ipVersion != ip) {
        Log(WARN) << "Cannot match IP version";
    }
}

Socket::~Socket() {
    Log(TRACE) << "Socket destructing";
}

void Socket::close() {
    Log(TRACE) << "Socket closed";
    ::close(fd);
}

}}
