#ifndef COLLIE_TCP_TCPCONNECTION_H
#define COLLIE_TCP_TCPCONNECTION_H

#include <string>
#include <memory>


namespace Collie {

namespace Event {

class Channel;

}

class SocketAddress;

namespace Tcp {

class TcpConnection {
public:

    TcpConnection(std::shared_ptr<Event::Channel> channel,
                  std::shared_ptr<SocketAddress> localAddr,
                  std::shared_ptr<SocketAddress> remoteAddr);

    TcpConnection(const TcpConnection &) = delete;
    TcpConnection operator=(const TcpConnection &) = delete;
    ~TcpConnection();

    std::shared_ptr<Event::Channel> getChannel() const { return channel; }
    std::shared_ptr<SocketAddress> getLocalAddr() const { return localAddr; }
    std::shared_ptr<SocketAddress> getRemoteAddr() const { return remoteAddr; }

    std::string recv();
    void send(const std::string &);

private:

    std::shared_ptr<SocketAddress> localAddr;
    std::shared_ptr<SocketAddress> remoteAddr;
    std::shared_ptr<Event::Channel> channel;
};

}}

#endif /* COLLIE_TCP_TCPCONNECTION_H */
