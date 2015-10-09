#ifndef SOCKET_H
#define SOCKET_H

#include "../httpd.hpp"
#include "SocketAddress.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>


namespace MiniHttp { namespace Base {

class Socket {
public:
    explicit Socket(const unsigned& port);
    Socket(const Socket&) = delete;
    Socket operator=(const Socket&) = delete;
    ~Socket();

    int getFd() const {
        return fd;
    }
    static int socket(const unsigned& port);
    void listen() const;
    int accept(std::shared_ptr<SocketAddress>) const;
    void close();

private:
    const int fd;
    const unsigned port;
};

}}

#endif /* SOCKET_H */
