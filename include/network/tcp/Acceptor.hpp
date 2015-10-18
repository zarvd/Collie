#ifndef COLLIE_NETWORK_TCP_ACCEPTOR_H
#define COLLIE_NETWORK_TCP_ACCEPTOR_H

#include <memory>
#include <functional>


namespace Collie {

namespace Event {
class EventLoop;
class Channel;
}

namespace Network {

class SocketAddress;

namespace Tcp {

class TcpSocket;

class Acceptor {
public:
    using AcceptCallback = std::function<void(const int, std::shared_ptr<SocketAddress>)>;

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

    std::unique_ptr<TcpSocket> tcpSocket;
    std::shared_ptr<SocketAddress> localAddr;
    std::shared_ptr<Event::EventLoop> eventLoop;
    std::shared_ptr<Event::Channel> channel;
    AcceptCallback acceptCallback;
};

}}}

#endif /* COLLIE_NETWORK_TCP_ACCEPTOR_H */
