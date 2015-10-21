#ifndef COLLIE_TCP_ACCEPTOR_H
#define COLLIE_TCP_ACCEPTOR_H

#include <memory>
#include <functional>


namespace Collie {

namespace Event {
class EventLoop;
class Channel;
}

class SocketAddress;

namespace Tcp {

class TcpSocket;

class Acceptor {
public:
    using AcceptCallback = std::function<void(const unsigned & connFd,
                                              std::shared_ptr<SocketAddress> remoteAddr)>;

    Acceptor(std::shared_ptr<SocketAddress>, std::shared_ptr<Event::EventLoop>);
    Acceptor(const Acceptor &) = delete;
    Acceptor & operator=(const Acceptor &) = delete;
    ~Acceptor();
    std::shared_ptr<Event::Channel> getChannel() const {
        return channel;
    }
    void setAcceptCallback(const AcceptCallback & cb) {
        acceptCallback = cb;
    }
    void socket();
    void accept();

private:

    void handleRead();
    void handleError();

    std::unique_ptr<TcpSocket> tcpSocket;
    std::shared_ptr<SocketAddress> localAddr;
    std::shared_ptr<Event::EventLoop> eventLoop;
    std::shared_ptr<Event::Channel> channel;
    AcceptCallback acceptCallback;
};

}}

#endif /* COLLIE_TCP_ACCEPTOR_H */
