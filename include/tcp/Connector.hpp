#ifndef COLLIE_TCP_CONNECTOR_H
#define COLLIE_TCP_CONNECTOR_H

#include <functional>
#include <memory>

namespace Collie {

namespace Event {

class EventLoop;

}

class SocketAddress;

namespace Tcp {

class TcpSocket;
class TcpConnection;

class Connector {
public:
    using ConnectCallback = std::function<void(std::shared_ptr<TcpConnection>)>;

    explicit Connector(std::shared_ptr<SocketAddress>, std::shared_ptr<Event::EventLoop>);
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
    std::shared_ptr<Event::EventLoop> eventLoop;
    std::shared_ptr<SocketAddress> remoteAddr;
    ConnectCallback connectCallback;
};

}}

#endif /* COLLIE_TCP_CONNECTOR_H */
