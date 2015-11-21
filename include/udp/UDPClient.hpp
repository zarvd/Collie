#ifndef COLLIE_UDP_UDPCLIENT_H
#define COLLIE_UDP_UDPCLIENT_H

#include <string>
#include <memory>
#include <functional>
#include "../Type.hpp"

namespace Collie {

class InetAddress;

namespace UDP {

class UDPSocket;

class UDPClient {
public:
    using ConnectCallback =
        std::function<bool(const String & content,
                           SharedPtr<InetAddress> remoteAddr)>;

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
    void connect(const String & host, const unsigned port,
                 const String & greeting);
    void send(const String & content,
              SharedPtr<InetAddress> remoteAddr);

private:
    UniquePtr<UDPSocket> socket;
    SharedPtr<InetAddress> remoteAddr;
    ConnectCallback connectCallback;
};
}
}

#endif /* COLLIE_UDP_UDPCLIENT_H */
