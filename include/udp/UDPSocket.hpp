#ifndef COLLIE_UDP_UDPSOCKET_H
#define COLLIE_UDP_UDPSOCKET_H

#include <memory>
#include "../Type.hpp"

namespace Collie {

enum class IP;
class InetAddress;

namespace UDP {

class UDPSocket {
public:
    explicit UDPSocket(SharedPtr<InetAddress>);
    UDPSocket(const UDPSocket &) = delete;
    UDPSocket & operator=(const UDPSocket &) = delete;
    ~UDPSocket();

    // server
    void listen();
    void listenV4();
    void listenV6();

    // client
    void connect(const IP); // only socket, UDP have NO connection
    void connectV4();
    void connectV6();

private:
    int fd;
    SharedPtr<InetAddress> address;
};
}
}

#endif /* COLLIE_UDP_UDPSOCKET_H */
