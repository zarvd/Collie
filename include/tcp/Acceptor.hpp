#ifndef COLLIE_TCP_ACCEPTOR_H
#define COLLIE_TCP_ACCEPTOR_H

#include <memory>
#include <functional>
#include <vector>

namespace Collie {

class SocketAddress;

namespace Event {
class Channel;
class EventLoop;
}

namespace Tcp {

class TcpSocket;

/**
 * Acceptor owns the socket fd
 */
class Acceptor final {
public:
    using AcceptCallback = std::function<void(
        const unsigned connFd, std::shared_ptr<SocketAddress> remoteAddr)>;

    explicit Acceptor(std::shared_ptr<SocketAddress>);
    Acceptor(const Acceptor &) = delete;
    Acceptor & operator=(const Acceptor &) = delete;
    ~Acceptor();

    // setter
    void setThreadNum(const size_t threadNum);
    void setAcceptCallback(const AcceptCallback & cb) { acceptCallback = cb; }
    void setAcceptCallback(const AcceptCallback && cb) {
        acceptCallback = std::move(cb);
    }

    // getter
    int getSocketFd() const;
    std::shared_ptr<Event::Channel> getBaseChannel();

    void socket();

private:
    void handleRead();
    void handleError();

    size_t threadNum;
    std::unique_ptr<TcpSocket> tcpSocket;
    std::shared_ptr<SocketAddress> localAddr;
    std::vector<Event::EventLoop> eventLoops;
    AcceptCallback acceptCallback;
};
}
}

#endif /* COLLIE_TCP_ACCEPTOR_H */
