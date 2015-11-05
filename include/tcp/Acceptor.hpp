#ifndef COLLIE_TCP_ACCEPTOR_H
#define COLLIE_TCP_ACCEPTOR_H

#include <memory>
#include <functional>

namespace Collie {

class SocketAddress;

namespace Event {
class Channel;
}

namespace Tcp {

class TcpSocket;

class Acceptor {
public:
    using AcceptCallback = std::function<void(
        const unsigned connFd, std::shared_ptr<SocketAddress> remoteAddr)>;

    explicit Acceptor(std::shared_ptr<SocketAddress>);
    Acceptor(const Acceptor &) = delete;
    Acceptor & operator=(const Acceptor &) = delete;
    ~Acceptor();
    void setAcceptCallback(const AcceptCallback & cb) { acceptCallback = cb; }
    void setAcceptCallback(const AcceptCallback && cb) {
        acceptCallback = std::move(cb);
    }
    void socket();
    int getSocketFd() const;
    std::shared_ptr<Event::Channel> getBaseChannel();

private:
    void handleRead();
    void handleError();

    std::unique_ptr<TcpSocket> tcpSocket;
    std::shared_ptr<SocketAddress> localAddr;
    AcceptCallback acceptCallback;
};
}
}

#endif /* COLLIE_TCP_ACCEPTOR_H */
