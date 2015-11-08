#ifndef COLLIE_TCP_TCPSERVER_H
#define COLLIE_TCP_TCPSERVER_H

#include <memory>
#include <string>
#include <unordered_set>

namespace Collie {

namespace Event {
class EventLoopThreadPool;
}

class SocketAddress;

namespace Tcp {

class Acceptor;
class TcpConnection;

class TcpServer {
public:
    using OnMessageCallback =
        std::function<void(std::shared_ptr<TcpConnection>)>;
    using ConnectedCallback = std::function<void()>;

    TcpServer();
    TcpServer(const TcpServer &) = delete;
    TcpServer & operator=(const TcpServer &) = delete;
    ~TcpServer();

    void start();
    void bind(const std::string & host, const unsigned port);
    void setThreadNum(const size_t threadNum = 1) {
        this->threadNum = threadNum;
    }
    // setter
    void setConnectedCallback(const ConnectedCallback & cb) {
        connectedCallback = cb;
    }
    void setConnectedCallback(const ConnectedCallback && cb) {
        connectedCallback = std::move(cb);
    }
    void setOnMessageCallback(const OnMessageCallback & cb) {
        onMessageCallback = cb;
    }
    void setOnMessageCallback(const OnMessageCallback && cb) {
        onMessageCallback = std::move(cb);
    }
    // getter
    unsigned getPort() const { return port; }

private:
    void newConnection(const unsigned connFd,
                       std::shared_ptr<SocketAddress> remoteAddr);
    void newConnectionMultiThread(const unsigned connFd,
                                  std::shared_ptr<SocketAddress> remoteAddr);

    size_t threadNum;
    std::unordered_set<std::shared_ptr<TcpConnection>> connections;
    std::string host;
    unsigned port;
    std::shared_ptr<SocketAddress> localAddr;
    std::unique_ptr<Event::EventLoopThreadPool> eventLoopThreadPool;
    std::unique_ptr<Acceptor> acceptor;
    ConnectedCallback connectedCallback;
    OnMessageCallback onMessageCallback;
};
}
}

#endif /* COLLIE_TCP_TCPSERVER_H */
