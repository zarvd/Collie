#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <memory>


namespace MiniHttp { namespace Base {

class Acceptor;

class EventLoop;

class TcpServer {
public:
    typedef std::function<void(int)> ConnectCallback;

    explicit TcpServer(const unsigned & port);
    TcpServer(const TcpServer &) = delete;
    TcpServer & operator=(const TcpServer &) = delete;
    ~TcpServer();

    void listen();
    void start();
    void setConnectCallback(ConnectCallback & cb) {
        connectCallback = cb;
    }
    unsigned getPort() const { return port; }

private:
    const unsigned port;
    std::unique_ptr<Acceptor> acceptor;
    std::shared_ptr<EventLoop> eventLoop;
    ConnectCallback connectCallback;
};

}}


#endif /* TCPSERVER_H */
