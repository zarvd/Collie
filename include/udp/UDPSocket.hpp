#ifndef COLLIE_UDP_UDPSOCKET_H
#define COLLIE_UDP_UDPSOCKET_H

#include "../Socket.hpp"
#include <memory>

namespace Collie {

class SocketAddress;

namespace UDP {

class UDPSocket : public Socket {
public:
    UDPSocket();                                        // client
    explicit UDPSocket(std::shared_ptr<SocketAddress>); // server
    UDPSocket(const UDPSocket &) = delete;
    UDPSocket & operator=(const UDPSocket &) = delete;
    ~UDPSocket() override;

    void listen() override;
    void listenV4();
    void listenV6();

    void connect(IP); // only socket, UDP have NO connection
    void connectV4();
    void connectV6();
};
}
}

#endif /* COLLIE_UDP_UDPSOCKET_H */
