#ifndef COLLIE_UDP_UDPCLIENT_H
#define COLLIE_UDP_UDPCLIENT_H

#include <string>
#include <memory>
#include <functional>

namespace Collie {

class SocketAddress;

namespace UDP {

class UDPSocket;

class UDPClient {
public:
    using ConnectCallback =
        std::function<bool(const std::string & content,
                           std::shared_ptr<SocketAddress> remoteAddr)>;

    UDPClient();
    UDPClient(const UDPClient &) = delete;
    UDPClient & operator=(const UDPClient &) = delete;
    ~UDPClient();

    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }
    void setConnectCallback(const ConnectCallback && cb) {
        connectCallback = std::move(cb);
    }
    void connect(const std::string & host, const unsigned port,
                 const std::string & greeting);
    void send(const std::string & content,
              std::shared_ptr<SocketAddress> remoteAddr);

private:
    std::unique_ptr<UDPSocket> socket;
    std::shared_ptr<SocketAddress> remoteAddr;
    ConnectCallback connectCallback;
};
}
}

#endif /* COLLIE_UDP_UDPCLIENT_H */
