#ifndef COLLIE_SOCKETADDRESS_H
#define COLLIE_SOCKETADDRESS_H

#include <string>
#include <memory>
#include <arpa/inet.h>


namespace Collie {

enum class IP;

class SocketAddress {
public:
    typedef struct sockaddr_in AddrV4;
    typedef struct sockaddr_in6 AddrV6;

    SocketAddress();
    explicit SocketAddress(const AddrV4 &);
    explicit SocketAddress(const AddrV6 &);
    SocketAddress(const SocketAddress &);
    ~SocketAddress();

    SocketAddress & operator=(const SocketAddress &);
    SocketAddress & operator=(const AddrV4 & addr);

    std::string getIP() const;
    IP getIPVersion() const;
    std::string getHostName() const;
    AddrV4 getAddrV4() const;
    AddrV6 getAddrV6() const;

    // TODO detect host's type(ip, domain) and ip version
    // then create sockaddr_in or sockaddr_in6
    // Note: it will block
    static std::shared_ptr<SocketAddress> getSocketAddress(const std::string & host, const unsigned & port);

private:

    std::string ip;
    IP ipVersion;
    AddrV4 addrV4;
    AddrV6 addrV6;
};

}

#endif /* COLLIE_SOCKETADDRESS_H */
