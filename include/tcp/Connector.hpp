#ifndef COLLIE_TCP_CONNECTOR_H
#define COLLIE_TCP_CONNECTOR_H

#include <functional>
#include <memory>

namespace Collie {

class SocketAddress;

namespace Tcp {

class TcpSocket;

class Connector {
public:
    using ConnectCallback = std::function<void(const unsigned &)>;

    explicit Connector(std::shared_ptr<SocketAddress>);
    Connector(const Connector &) = delete;
    Connector & operator=(const Connector &) = delete;
    ~Connector();

    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }

    void socket();
    void connect();

private:
    std::unique_ptr<TcpSocket> tcpSocket;
    std::shared_ptr<SocketAddress> remoteAddr;
    ConnectCallback connectCallback;
};

}}

#endif /* COLLIE_TCP_CONNECTOR_H */
