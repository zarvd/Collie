#ifndef COLLIE_UDP_UDPSERVER_H
#define COLLIE_UDP_UDPSERVER_H

#include <string>
#include <memory>
#include <functional>
#include "../Type.hpp"

namespace Collie {

class InetAddress;

namespace UDP {

class UDPSocket;

class UDPServer {
public:
    using ConnectCallback = std::function<void(
        const String & content, SharedPtr<InetAddress> remoteAddr)>;

    UDPServer();
    UDPServer(const UDPServer &) = delete;
    UDPServer & operator=(const UDPServer &) = delete;
    ~UDPServer();

    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }
    void setConnectCallback(const ConnectCallback && cb) {
        connectCallback = std::move(cb);
    }
    void bind(const String & host, const unsigned port);
    void start();
    void send(const String & content, SharedPtr<InetAddress> remoteAddr);

private:
    UniquePtr<UDPSocket> socket;
    SharedPtr<InetAddress> localAddr;
    ConnectCallback connectCallback;
};
}
}

#endif /* COLLIE_UDP_UDPSERVER_H */
