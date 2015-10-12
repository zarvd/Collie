#ifndef MINIHTTP_BASE_TCPCONNECTION_H
#define MINIHTTP_BASE_TCPCONNECTION_H

#include <string>
#include <memory>


namespace MiniHttp { namespace Base {

class Channel;
class SocketAddress;

class TcpConnection {
public:

    TcpConnection(const std::string & content,
                  std::shared_ptr<SocketAddress> localAddr,
                  std::shared_ptr<SocketAddress> remoteAddr,
                  std::shared_ptr<Channel> channel);

    TcpConnection(const TcpConnection &) = delete;
    TcpConnection operator=(const TcpConnection &) = delete;
    ~TcpConnection();

    std::shared_ptr<SocketAddress> getLocalAddr() const {
        return localAddr;
    }
    std::shared_ptr<SocketAddress> getRemoteAddr() const {
        return remoteAddr;
    }
    std::shared_ptr<Channel> getChannel() const {
        return channel;
    }

    const std::string content;

private:

    std::shared_ptr<SocketAddress> localAddr;
    std::shared_ptr<SocketAddress> remoteAddr;
    std::shared_ptr<Channel> channel;
};

} }

#endif /* MINIHTTP_BASE_TCPCONNECTION_H */
