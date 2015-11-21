#include <netdb.h>
#include "../include/Global.hpp"
#include "../include/InetAddress.hpp"

namespace Collie {

InetAddress::InetAddress() : ip(), port() {}
InetAddress::~InetAddress() {}

InetAddress & InetAddress::operator=(const InetAddress & that) {
    ip = that.ip;
    port = that.port;
    ipVersion = that.ipVersion;
    addrV4 = that.addrV4;
    addrV6 = that.addrV6;
    return *this;
}

InetAddress & InetAddress::operator=(const AddrV4 & addr) {
    ipVersion = IP::V4;
    addrV4 = addr;
    char ipv4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addrV4.sin_addr), ipv4, INET_ADDRSTRLEN);
    ip = ipv4;
    port = ntohs(addr.sin_port);
    return *this;
}

InetAddress & InetAddress::operator=(const AddrV6 &) {
    // TODO
    return *this;
}

std::shared_ptr<InetAddress>
InetAddress::getInetAddress(const std::string & host, const unsigned port) {

    auto addr = std::make_shared<InetAddress>();

    struct hostent * hostStruct = gethostbyname(host.c_str());
    if(!hostStruct) {
        THROW_(hstrerror(h_errno));
    } else {
        switch(hostStruct->h_addrtype) {
        case AF_INET:
            // IPv4
            char ipv4[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET, *(hostStruct->h_addr_list), ipv4,
                         INET_ADDRSTRLEN) == NULL) {
                THROW_SYS;
            }
            addr->ip = ipv4;
            addr->port = port;
            addr->ipVersion = IP::V4;
            addr->addrV4.sin_family = AF_INET;
            addr->addrV4.sin_port = htons(port);
            if(inet_pton(AF_INET, addr->ip.c_str(), &(addr->addrV4.sin_addr)) !=
               1) {
                THROW_SYS;
            }
            break;
        case AF_INET6:
            // IPv6
            char ipv6[INET6_ADDRSTRLEN];
            if(inet_ntop(AF_INET6, *(hostStruct->h_addr_list), ipv6,
                         INET6_ADDRSTRLEN) == NULL) {
                THROW_SYS;
            }
            addr->ip = ipv6;
            addr->port = port;
            addr->ipVersion = IP::V6;
            addr->addrV6.sin6_family = AF_INET6;
            addr->addrV6.sin6_port = htons(port);
            if(inet_pton(AF_INET6, addr->ip.c_str(),
                         &(addr->addrV6.sin6_addr)) != 1) {
                THROW_SYS;
            }
            break;
        default:
            break;
        }
    }

    return addr;
}
}
