#ifndef COLLIE_NETWORK_TCP_TCPCLIENT_H
#define COLLIE_NETWORK_TCP_TCPCLIENT_H

#include <functional>
#include <memory>
#include <string>

namespace Collie { namespace Network {

class SocketAddress;

namespace Tcp {

class Connector;

class TcpClient {
public:
    using ConnectCallback = std::function<void(const unsigned &)>;

    TcpClient();
    TcpClient(const TcpClient &) = delete;
    TcpClient & operator=(const TcpClient &) = delete;
    ~TcpClient();

    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }
    void connect(const std::string & host, const unsigned & port);

private:
    std::unique_ptr<Connector> connector;
    ConnectCallback connectCallback;
    std::shared_ptr<SocketAddress> remoteAddr;
};

}}}

#endif /* COLLIE_NETWORK_TCP_TCPCLIENT_H */
