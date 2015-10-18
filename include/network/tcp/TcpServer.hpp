#ifndef COLLIE_NETWORK_TCP_TCPSERVER_H
#define COLLIE_NETWORK_TCP_TCPSERVER_H

#include <memory>


namespace Collie {

namespace Base {
class EventLoop;
}

namespace Network { namespace Tcp {

class Acceptor;

class TcpServer {
public:
    using ConnectCallback = std::function<void(const unsigned)>;

    explicit TcpServer(const unsigned port);
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
    std::shared_ptr<Base::EventLoop> getEventLoop() const { return eventLoop; }

private:
    void newConnection();


    const unsigned port;
    std::shared_ptr<Base::EventLoop> eventLoop;
    std::unique_ptr<Acceptor> acceptor;
    ConnectCallback messageCallback;
    ConnectCallback connectCallback;
};

}}}

#endif /* COLLIE_NETWORK_TCP_TCPSERVER_H */
