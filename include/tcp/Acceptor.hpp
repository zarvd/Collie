#ifndef COLLIE_TCP_ACCEPTOR_H
#define COLLIE_TCP_ACCEPTOR_H

#include <memory>
#include <functional>
#include <vector>
#include "../Type.hpp"

namespace Collie {

class InetAddress;
class Descriptor;

namespace Event {
class Channel;
class EventLoop;
}

namespace TCP {

class TCPSocket;

/**
 * Acceptor owns the socket fd
 */
class Acceptor {
public:
    using AcceptCallback = std::function<void(SharedPtr<TCPSocket> connSocket)>;

    explicit Acceptor(SharedPtr<InetAddress>) noexcept;
    Acceptor(const Acceptor &) = delete;
    Acceptor & operator=(const Acceptor &) = delete;
    ~Acceptor() noexcept;

    // setter
    void setThreadNum(const size_t threadNum);
    void setAcceptCallback(const AcceptCallback & cb) noexcept {
        acceptCallback = cb;
    }
    void setAcceptCallback(const AcceptCallback && cb) noexcept {
        acceptCallback = std::move(cb);
    }

    // getter
    SharedPtr<Event::Channel> getBaseChannel();

private:
    void handleRead();
    void handleError();

    size_t threadNum;
    SharedPtr<InetAddress> localAddr;
    SharedPtr<TCPSocket> tcpSocket; // listen socket
    Vec<Event::EventLoop> eventLoops;
    AcceptCallback acceptCallback;
};
}
}

#endif /* COLLIE_TCP_ACCEPTOR_H */
