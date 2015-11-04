#ifndef COLLIE_TCP_TCPSERVER_H
#define COLLIE_TCP_TCPSERVER_H

#include <memory>
#include <string>
#include <set>

namespace Collie {

namespace Event {
class EventLoop;
class ThreadPool;
}

class SocketAddress;

namespace Tcp {

class Acceptor;
class TcpConnection;

class TcpServer {
public:
    using OnMessageCallback = std::function<void(std::shared_ptr<TcpConnection>)>;
    using ConnectedCallback = std::function<void()>;

    TcpServer(const std::string & host, const unsigned port);
    TcpServer(const TcpServer &) = delete;
    TcpServer & operator=(const TcpServer &) = delete;
    ~TcpServer();

    void start();
    void multiThread(const int threadNum = 1);
    // setter
    void setConnectedCallback(const ConnectedCallback & cb) { connectedCallback = cb; }
    void setConnectedCallback(const ConnectedCallback && cb) { connectedCallback = std::move(cb); }
    void setOnMessageCallback(const OnMessageCallback & cb) { onMessageCallback = cb; }
    void setOnMessageCallback(const OnMessageCallback && cb) { onMessageCallback = std::move(cb); }
    // getter
    unsigned getPort() const { return port; }
    std::shared_ptr<Event::EventLoop> getEventLoop() const { return eventLoop; }

private:
    void newConnection(const unsigned connFd,
                       std::shared_ptr<SocketAddress> remoteAddr);
    void newConnectionMultiThread(const unsigned connFd,
                                  std::shared_ptr<SocketAddress> remoteAddr);

    bool isMultiThread;
    std::set<std::shared_ptr<TcpConnection> > connections;
    const std::string host;
    const unsigned port;
    std::shared_ptr<SocketAddress> localAddr;
    std::shared_ptr<Event::EventLoop> eventLoop;
    std::unique_ptr<Acceptor> acceptor;
    std::shared_ptr<Event::ThreadPool> threadPool;
    ConnectedCallback connectedCallback;
    OnMessageCallback onMessageCallback;
};

}}

#endif /* COLLIE_TCP_TCPSERVER_H */
