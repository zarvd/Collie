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
    Socket(const unsigned & port,
           std::shared_ptr<SocketAddress>);
    Socket(const Socket &) = delete;
    Socket & operator=(const Socket&) = delete;
    virtual ~Socket() = 0;

    // getter
    int getFd() const { return fd; }
    int getPort() const { return port; }
    std::shared_ptr<SocketAddress> getAddr() const { return addr; }

    virtual void listen();
    void setNonBlocking();
    void setBlocking();
    void close();

protected:

    const IP ipVersion;
    int fd;
    const unsigned port;
    std::shared_ptr<SocketAddress> addr;
};

}}

#endif /* COLLIE_NETWORK_SOCKET_H */
