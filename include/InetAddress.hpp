#ifndef COLLIE_INETADDRESS_H
#define COLLIE_INETADDRESS_H

#include <string>
#include <memory>
#include <arpa/inet.h>

namespace Collie {

enum class IP;

class InetAddress {
public:
    using AddrV4 = struct sockaddr_in;
    using AddrV6 = struct sockaddr_in6;

    InetAddress();
    explicit InetAddress(const AddrV4 &);
    explicit InetAddress(const AddrV6 &);
    InetAddress(const InetAddress &);
    ~InetAddress();

    InetAddress & operator=(const InetAddress &);
    InetAddress & operator=(const AddrV4 &);
    InetAddress & operator=(const AddrV6 &);

    std::string getIP() const { return ip; }
    unsigned getPort() const { return port; }
    IP getIPVersion() const { return ipVersion; }
    AddrV4 getAddrV4() const { return addrV4; }
    AddrV6 getAddrV6() const { return addrV6; }

    // TODO detect host's type(ip, domain) and ip version
    // then create sockaddr_in or sockaddr_in6
    // Note: it will block
    static std::shared_ptr<InetAddress> getInetAddress(const std::string & host,
                                                       const unsigned port);

private:
    std::string ip;
    unsigned port;
    IP ipVersion;
    AddrV4 addrV4;
    AddrV6 addrV6;
};
}

#endif /* COLLIE_INETADDRESS_H */
