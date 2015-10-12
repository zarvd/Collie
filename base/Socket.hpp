#ifndef MINIHTTP_BASE_SOCKET_H
#define MINIHTTP_BASE_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory>


namespace MiniHttp { namespace Base {

class SocketAddress;

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
    std::shared_ptr<SocketAddress> getAddr() const {
        return addr;
    }
    int socket(const unsigned& port);
    void listen() const;
    int accept(std::shared_ptr<SocketAddress>) const;
    static int accept(const int & fd, std::shared_ptr<SocketAddress>);
    static std::string recv(const int & fd);
    static void send(const int & fd, const std::string & msg);
    void close();

private:
    const int fd;
    const unsigned port;
    std::shared_ptr<SocketAddress> addr;
};

}}

#endif /* MINIHTTP_BASE_SOCKET_H */
