#ifndef COLLIE_SOCKET_H
#define COLLIE_SOCKET_H

#include <memory>

namespace Collie {

enum class IP;
class SocketAddress;

/**
 * Abstract class
 */
class Socket {
public:
    enum class Type { Client, Server };

    Socket();                                        // client constructor
    explicit Socket(std::shared_ptr<SocketAddress>); // server constructor
    Socket(const Socket &) = delete;
    Socket & operator=(const Socket &) = delete;
    virtual ~Socket() = 0;

    // getter
    int getFd() const { return fd; }
    std::shared_ptr<SocketAddress> getLocalAddr() const { return localAddr; }

    virtual void listen() = 0;
    static void setFdNonBlocking(int fd);
    static ssize_t send(const int socketFd, const std::string & content,
                        const int flags = 0);
    static ssize_t sendTo(const int socketFd, const std::string & content,
                          std::shared_ptr<SocketAddress> remoteAddr,
                          const int flags = 0);
    // received content will store in std::string content
    static ssize_t recv(const int socketFd, std::string & content,
                        const int flags = 0);
    static ssize_t recvFrom(const int socketFd, std::string & content,
                            const std::shared_ptr<SocketAddress> & remoteAddr,
                            const int flags = 0);
    ssize_t send(const std::string & content, const int flags = 0);
    ssize_t recv(std::string & content, const int flags = 0);
    ssize_t sendTo(const std::string & content,
                   std::shared_ptr<SocketAddress> remoteAddr,
                   const int flags = 0);
    ssize_t recvFrom(std::string & content,
                     const std::shared_ptr<SocketAddress> & remoteAddr,
                     const int flags = 0);
    ssize_t sendFile(const int socketFd, const std::string & fileName);
    ssize_t recvFile(const int socketFd, const std::string & fileName);
    void close();

protected:
    const Type type;
    int fd;
    std::shared_ptr<SocketAddress> localAddr;
};
}

#endif /* COLLIE_SOCKET_H */
