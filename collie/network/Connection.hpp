#ifndef COLLIE_NETWORK_CONNECTION_H
#define COLLIE_NETWORK_CONNECTION_H

#include <string>
#include <memory>


namespace Collie { namespace Network {

class SocketAddress;

/**
 * Abstract class
 */
class Connection {
public:

    Connection(std::shared_ptr<SocketAddress> localAddr,
               std::shared_ptr<SocketAddress> remoteAddr);

    Connection(const Connection &) = delete;
    Connection & operator=(const Connection &) = delete;
    virtual ~Connection() = 0;

    std::shared_ptr<SocketAddress> getLocalAddr() const {
        return localAddr;
    }

    std::shared_ptr<SocketAddress> getRemoteAddr() const {
        return remoteAddr;
    }

    virtual std::string recv();  // FIXME recv other type
    virtual void send(const std::string &); // FIXME send other type

private:

    std::shared_ptr<SocketAddress> localAddr;
    std::shared_ptr<SocketAddress> remoteAddr;
};

} }

#endif /* COLLIE_NETWORK_CONNECTION_H */
