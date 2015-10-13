#ifndef COLLIE_NETWORK_SOCKETADDRESS_H
#define COLLIE_NETWORK_SOCKETADDRESS_H

#include <string>
#include <arpa/inet.h>


namespace Collie { namespace Network {

enum class IP;

class SocketAddress {
public:
    typedef struct sockaddr_in AddrV4;
    typedef struct sockaddr_in6 AddrV6;

    explicit SocketAddress(const AddrV4 &);
    explicit SocketAddress(const AddrV6 &);
    SocketAddress(const SocketAddress &);
    ~SocketAddress();

    SocketAddress & operator=(const SocketAddress &);
    SocketAddress & operator=(const AddrV4 & addr);

    std::string getIP() const;
    std::string getHostName() const;
    AddrV4 getAddrV4() const;
    AddrV6 getAddrV6() const;

    // TODO detect host's type(ip, domain) and ip version
    // then create sockaddr_in or sockaddr_in6
    static SocketAddress getSocketAddress(const std::string & host,
                                          const unsigned & port);

    const IP ipVersion;

private:

    AddrV4 addrV4;
    AddrV6 addrV6;
};

}}

#endif /* COLLIE_NETWORK_SOCKETADDRESS_H */
