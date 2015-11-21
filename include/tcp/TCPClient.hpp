#ifndef COLLIE_TCP_TCPCLIENT_H
#define COLLIE_TCP_TCPCLIENT_H

#include <functional>
#include <memory>
#include <string>
#include "../Type.hpp"

namespace Collie {

class InetAddress;

namespace TCP {

class Connector;
class TCPSocket;

class TCPClient {
public:
    using ConnectCallback = std::function<void(SharedPtr<TCPSocket>)>;

    TCPClient();
    TCPClient(const TCPClient &) = delete;
    TCPClient & operator=(const TCPClient &) = delete;
    ~TCPClient();

    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }
    void setConnectCallback(const ConnectCallback && cb) {
        connectCallback = std::move(cb);
    }
    void connect(const String & host, const unsigned port,
                 const size_t threadNum = 1, const size_t connectNum = 1);

private:
    UniquePtr<Connector> connector;
    ConnectCallback connectCallback;
    SharedPtr<InetAddress> remoteAddr;
};
}
}

#endif /* COLLIE_TCP_TCPCLIENT_H */
