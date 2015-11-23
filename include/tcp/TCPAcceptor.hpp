#ifndef COLLIE_TCP_TCPACCEPTOR_H
#define COLLIE_TCP_TCPACCEPTOR_H

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

class TCPAcceptor {
public:
    using AcceptCallback = std::function<void(SharedPtr<TCPSocket> connSocket)>;

    explicit TCPAcceptor(SharedPtr<InetAddress>) noexcept;
    TCPAcceptor(const TCPAcceptor &) = delete;
    TCPAcceptor & operator=(const TCPAcceptor &) = delete;
    ~TCPAcceptor() noexcept;

    void bindAndListen() const;

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

#endif /* COLLIE_TCP_TCPACCEPTOR_H */
