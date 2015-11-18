#ifndef COLLIE_TCP_TCPCONNECTION_H
#define COLLIE_TCP_TCPCONNECTION_H

#include <string>
#include <memory>
#include <list>
#include <unordered_set>

namespace Collie {

namespace Event {

class EventLoop;
class Channel;
}

class SocketAddress;

namespace TCP {

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
public:
    using MessageCallback = std::function<void(std::shared_ptr<TCPConnection>)>;
    using EventCallback = std::function<void(std::shared_ptr<TCPConnection>)>;

    TCPConnection(std::shared_ptr<Event::Channel> channel,
                  std::shared_ptr<const SocketAddress> localAddr,
                  std::shared_ptr<const SocketAddress> remoteAddr);
    TCPConnection(const TCPConnection &) = delete;
    TCPConnection & operator=(const TCPConnection &) = delete;
    ~TCPConnection();

    std::shared_ptr<Event::Channel> getChannel() const { return channel; }
    std::shared_ptr<const SocketAddress> getLocalAddr() const { return localAddr; }
    std::shared_ptr<const SocketAddress> getRemoteAddr() const { return remoteAddr; }
    void setMessageCallback(const MessageCallback & cb) {
        messageCallback = cb;
    }
    void setMessageCallback(const MessageCallback && cb) {
        messageCallback = std::move(cb);
    }

    void setShutdownCallback(const EventCallback & cb) {
        shutdownCallback = cb;
    }
    void setShutdownCallback(const EventCallback && cb) {
        shutdownCallback = std::move(cb);
    }

    void disconnect();
    std::string recvAll();
    void send(const std::string &);

private:
    void shutdown();
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    bool connected;
    bool isShutDown;
    const std::shared_ptr<Event::Channel> channel;
    const std::shared_ptr<const SocketAddress> localAddr;
    const std::shared_ptr<const SocketAddress> remoteAddr;
    std::string inputBuffer;
    std::string outputBuffer;
    MessageCallback messageCallback;
    EventCallback shutdownCallback;
};

extern thread_local std::unordered_set<std::shared_ptr<TCPConnection>>
    localThreadConnections;
}
}

#endif /* COLLIE_TCP_TCPCONNECTION_H */
