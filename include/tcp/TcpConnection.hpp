#ifndef COLLIE_TCP_TCPCONNECTION_H
#define COLLIE_TCP_TCPCONNECTION_H

#include <string>
#include <memory>
#include <list>


namespace Collie {

namespace Event {

class EventLoop;
class Channel;

}

class SocketAddress;

namespace Tcp {

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    using MessageCallback = std::function<void(std::shared_ptr<TcpConnection>)>;
    using EventCallback = std::function<void(std::shared_ptr<TcpConnection>)>;

    TcpConnection(std::shared_ptr<Event::EventLoop> eventLoop,
                  const unsigned fd,
                  std::shared_ptr<SocketAddress> localAddr,
                  std::shared_ptr<SocketAddress> remoteAddr);
    TcpConnection(const TcpConnection &) = delete;
    TcpConnection & operator=(const TcpConnection &) = delete;
    ~TcpConnection();

    std::shared_ptr<Event::Channel> getChannel() const { return channel; }
    std::shared_ptr<SocketAddress> getLocalAddr() const { return localAddr; }
    std::shared_ptr<SocketAddress> getRemoteAddr() const { return remoteAddr; }
    void setMessageCallback(const MessageCallback & cb) { messageCallback = cb; }
    void setMessageCallback(const MessageCallback && cb) { messageCallback = std::move(cb); }

    void setShutdownCallback(const EventCallback & cb) { shutdownCallback = cb; }
    void setShutdownCallback(const EventCallback && cb) { shutdownCallback = std::move(cb); }

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
    const unsigned connFd;
    std::shared_ptr<Event::EventLoop> eventLoop;
    std::shared_ptr<Event::Channel> channel;
    std::shared_ptr<SocketAddress> localAddr;
    std::shared_ptr<SocketAddress> remoteAddr;
    std::string inputBuffer;
    std::string outputBuffer;
    MessageCallback messageCallback;
    EventCallback shutdownCallback;
};

}}

#endif /* COLLIE_TCP_TCPCONNECTION_H */
