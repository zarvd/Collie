#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <memory>
#include "Callback.hpp"


namespace MiniHttp { namespace Base {

class Acceptor;
class EventLoop;

class TcpServer {
public:
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
    std::shared_ptr<EventLoop> getEventLoop() const { return eventLoop; }

private:
    void newConnection();


    const unsigned port;
    std::shared_ptr<EventLoop> eventLoop;
    std::unique_ptr<Acceptor> acceptor;
    ConnectCallback messageCallback;
    ConnectCallback connectCallback;
};

}}


#endif /* TCPSERVER_H */
