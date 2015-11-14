#ifndef COLLIE_UDP_UDPSERVER_H
#define COLLIE_UDP_UDPSERVER_H

#include <string>
#include <memory>
#include <functional>

namespace Collie {

class SocketAddress;

namespace UDP {

class UDPSocket;

class UDPServer {
public:
    using ConnectCallback =
        std::function<void(const std::string & content,
                           std::shared_ptr<SocketAddress> remoteAddr)>;

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
    void bind(const std::string & host, const unsigned port);
    void start();
    void send(const std::string & content,
              std::shared_ptr<SocketAddress> remoteAddr);

private:
    std::unique_ptr<UDPSocket> socket;
    std::shared_ptr<SocketAddress> localAddr;
    ConnectCallback connectCallback;
};
}
}

#endif /* COLLIE_UDP_UDPSERVER_H */
