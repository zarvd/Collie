#ifndef COLLIE_TCP_TCPSERVER_H
#define COLLIE_TCP_TCPSERVER_H

#include <memory>
#include <string>

namespace Collie {

namespace Event {
class EventLoop;
}

class SocketAddress;

namespace Tcp {

class Acceptor;
class TcpConnection;

class TcpServer {
public:
    using ConnectCallback = std::function<void(std::shared_ptr<TcpConnection>)>;

    explicit TcpServer(const std::string & host, const unsigned & port);
    TcpServer(const TcpServer &) = delete;
    TcpServer & operator=(const TcpServer &) = delete;
    ~TcpServer();

    void start();
    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }
    void setMessageCallback(const ConnectCallback & cb) {
        messageCallback = cb;
    }
    unsigned getPort() const { return port; }
    std::shared_ptr<Event::EventLoop> getEventLoop() const { return eventLoop; }

private:
    void newConnection();

    const std::string host;
    const unsigned port;
    std::shared_ptr<SocketAddress> localAddr;
    std::shared_ptr<Event::EventLoop> eventLoop;
    std::unique_ptr<Acceptor> acceptor;
    ConnectCallback connectCallback;
    ConnectCallback messageCallback;
};

}}

#endif /* COLLIE_TCP_TCPSERVER_H */
