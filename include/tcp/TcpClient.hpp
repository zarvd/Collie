#ifndef COLLIE_TCP_TCPCLIENT_H
#define COLLIE_TCP_TCPCLIENT_H

#include <functional>
#include <memory>
#include <string>

namespace Collie {

class SocketAddress;

namespace Tcp {

class Connector;

class TcpClient {
public:
    using ConnectCallback = std::function<void()>;

    TcpClient();
    TcpClient(const TcpClient &) = delete;
    TcpClient & operator=(const TcpClient &) = delete;
    ~TcpClient();

    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }
    void setConnectCallback(const ConnectCallback && cb) {
        connectCallback = std::move(cb);
    }
    void connect(const std::string & host, const unsigned & port);

private:
    std::unique_ptr<Connector> connector;
    ConnectCallback connectCallback;
    std::shared_ptr<SocketAddress> remoteAddr;
};
}
}

#endif /* COLLIE_TCP_TCPCLIENT_H */
