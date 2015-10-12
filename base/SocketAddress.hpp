#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <arpa/inet.h>


namespace MiniHttp { namespace Base {

class SocketAddress {
public:
    typedef struct sockaddr_in Addr;

    SocketAddress() {}
    explicit SocketAddress(const Addr & addr) : addr(addr) {}
    SocketAddress(const SocketAddress & socketAddr) : addr(socketAddr.addr) {}
    ~SocketAddress() {}

    SocketAddress & operator=(const SocketAddress & socketAddr) {
        addr = socketAddr.addr;
        return * this;
    }
    SocketAddress & operator=(const Addr & addr) {
        this->addr = addr;
        return * this;
    }

    std::string getIP() const;
    std::string getHostName() const;

private:

    struct sockaddr_in addr;
};

}}

#endif /* CONNECTION_H */
