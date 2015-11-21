#ifndef COLLIE_TCP_CONNECTOR_H
#define COLLIE_TCP_CONNECTOR_H

#include <functional>
#include <memory>
#include "../Type.hpp"

namespace Collie {

namespace Event {

class ThreadPool;

}

class InetAddress;

namespace TCP {

class TCPSocket;

/**
 * Based on thread pool
 */
class Connector {
public:
    using ConnectCallback = std::function<void(SharedPtr<TCPSocket>)>;

    explicit Connector(SharedPtr<InetAddress>);
    Connector(const Connector &) = delete;
    Connector & operator=(const Connector &) = delete;
    ~Connector();

    void setConnectCallback(const ConnectCallback & cb) {
        connectCallback = cb;
    }
    void setConnectCallback(const ConnectCallback && cb) {
        connectCallback = std::move(cb);
    }
    void connect(const size_t threadNum, const size_t connectNum);
    void disconnect();

private:
    UniquePtr<Event::ThreadPool> threadPool;
    SharedPtr<InetAddress> remoteAddr;
    ConnectCallback connectCallback;
};
}
}

#endif /* COLLIE_TCP_CONNECTOR_H */
