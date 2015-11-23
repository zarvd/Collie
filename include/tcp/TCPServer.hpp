#ifndef COLLIE_TCP_TCPSERVER_H
#define COLLIE_TCP_TCPSERVER_H

#include <thread>
#include <memory>
#include <string>
#include "../Type.hpp"

namespace Collie {

namespace Event {
class EventLoopThreadPool;
}

class InetAddress;

namespace TCP {

class TCPAcceptor;
class TCPConnection;
class TCPSocket;

class TCPServer {
public:
    using OnMessageCallback = std::function<void(SharedPtr<TCPConnection>)>;
    using ConnectedCallback = std::function<void()>;

    TCPServer();
    TCPServer(const TCPServer &) = delete;
    TCPServer & operator=(const TCPServer &) = delete;
    ~TCPServer();

    void start();
    void bind(const String & host, const unsigned port);
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
    void newConnection(SharedPtr<TCPSocket> connSocket);

    size_t threadNum;
    String host;
    unsigned port;
    SharedPtr<InetAddress> localAddr;
    UniquePtr<Event::EventLoopThreadPool> eventLoopThreadPool;
    UniquePtr<TCPAcceptor> acceptor;
    ConnectedCallback connectedCallback;
    OnMessageCallback onMessageCallback;
};
}
}

#endif /* COLLIE_TCP_TCPSERVER_H */
