#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>
#include "../Httpd.hpp"
#include "SocketAddress.hpp"


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
    int getPort() const {
        return port;
    }
    static int socket(const unsigned& port);
    void listen() const;
    int accept(std::shared_ptr<SocketAddress>) const;
    static int accept(const int & fd, std::shared_ptr<SocketAddress>);
    static std::string recv(const int & fd);
    static void send(const int & fd, const std::string & msg);
    void close();

private:
    const int fd;
    const unsigned port;
};

}}

#endif /* SOCKET_H */