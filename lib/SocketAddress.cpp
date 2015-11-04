#include <netdb.h>
#include "../include/Global.hpp"
#include "../include/SocketAddress.hpp"

namespace Collie {

SocketAddress::SocketAddress() : ip(), port() {}
SocketAddress::~SocketAddress() {}

SocketAddress & SocketAddress::operator=(const SocketAddress & that) {
    ip = that.ip;
    port = that.port;
    ipVersion = that.ipVersion;
    addrV4 = that.addrV4;
    addrV6 = that.addrV6;
    return *this;
}

SocketAddress & SocketAddress::operator=(const AddrV4 & addr) {
    // TODO
    ipVersion = IP::V4;
    addrV4 = addr;
    char ipv4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addrV4.sin_addr), ipv4, INET_ADDRSTRLEN);
    ip = ipv4;
    port = ntohs(addr.sin_port);
    return *this;
}

SocketAddress & SocketAddress::operator=(const AddrV6 &) {
    // TODO
    return *this;
}

std::shared_ptr<SocketAddress>
SocketAddress::getSocketAddress(const std::string & host,
                                const unsigned & port) {

    std::shared_ptr<SocketAddress> addr(new SocketAddress);

    struct hostent * hostStruct =
        gethostbyname(host.c_str()); // FIXME: Blocking
    if(!hostStruct) {
        Log(WARN) << hstrerror(h_errno);
        THROW_INVALID_ARGUMENT;
    } else {
        int ret = -1;
        switch(hostStruct->h_addrtype) {
        case AF_INET:
            // IPv4
            char ipv4[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, *(hostStruct->h_addr_list), ipv4,
                         INET_ADDRSTRLEN) == NULL) {
                Log(ERROR) << "inet_ntop IPv4 error " << Exception::getErr();
                THROW_INVALID_ARGUMENT;
            }
            addr->ip = ipv4;
            addr->port = port;
            addr->ipVersion = IP::V4;
            addr->addrV4.sin_family = AF_INET;
            addr->addrV4.sin_port = htons(port);
            ret =
                inet_pton(AF_INET, addr->ip.c_str(), &(addr->addrV4.sin_addr));
            if(ret != 1) {
                Log(ERROR) << "inet_pton IPv4 error " << Exception::getErr();
                THROW_INVALID_ARGUMENT;
            }
            break;
        case AF_INET6:
            // IPv6
            char ipv6[INET6_ADDRSTRLEN];
            if(inet_ntop(AF_INET6, *(hostStruct->h_addr_list), ipv6,
                         INET6_ADDRSTRLEN) == NULL) {
                Log(ERROR) << "inet_ntop IPv6 error " << Exception::getErr();
                THROW_INVALID_ARGUMENT;
            }
            addr->ip = ipv6;
            addr->port = port;
            addr->ipVersion = IP::V6;
            addr->addrV6.sin6_family = AF_INET6;
            addr->addrV6.sin6_port = htons(port);
            ret = inet_pton(AF_INET6, addr->ip.c_str(),
                            &(addr->addrV6.sin6_addr));
            if(ret != 1) {
                Log(ERROR) << "inet_pton IPv6 error " << Exception::getErr();
                THROW_INVALID_ARGUMENT;
            }
            break;
        default:
            break;
        }
    }

    return addr;
}
}
