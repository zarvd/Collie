#ifndef COLLIE_NETWORK_SOCKET_H
#define COLLIE_NETWORK_SOCKET_H

#include <sys/socket.h>
#include <memory>
#include <netinet/in.h>


namespace Collie { namespace Network {

enum class IP;
class SocketAddress;

/**
 * Abstract class
 */
class Socket {
public:
    enum class Type {Client, Server};

    Socket();  // client constructor
    explicit Socket(std::shared_ptr<SocketAddress>);  // server constructor
    Socket(const Socket &) = delete;
    Socket & operator=(const Socket&) = delete;
    virtual ~Socket() = 0;

    // getter
    int getFd() const { return fd; }
    std::shared_ptr<SocketAddress> getLocalAddr() const { return localAddr; }

    virtual void listen();
    virtual void connect(std::shared_ptr<SocketAddress>);
    void setNonBlocking();
    void setBlocking();
    void close();

protected:

    const Type type;
    int fd;
    std::shared_ptr<SocketAddress> localAddr;
};

}}

#endif /* COLLIE_NETWORK_SOCKET_H */
