#ifndef COLLIE_TCP_TCPSERVER_H
#define COLLIE_TCP_TCPSERVER_H

#include <thread>
#include <memory>
#include <string>

namespace Collie {

namespace Event {
class EventLoopThreadPool;
}

class SocketAddress;

namespace TCP {

class Acceptor;
class TCPConnection;

class TCPServer {
public:
    using OnMessageCallback =
        std::function<void(std::shared_ptr<TCPConnection>)>;
    using ConnectedCallback = std::function<void()>;

    TCPServer();
    TCPServer(const TCPServer &) = delete;
    TCPServer & operator=(const TCPServer &) = delete;
    ~TCPServer();

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
