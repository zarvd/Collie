#ifndef COLLIE_TCP_TCPSOCKET_H
#define COLLIE_TCP_TCPSOCKET_H

#include "../Socket.hpp"

namespace Collie {
namespace TCP {

class TCPSocket : public Socket {
public:
    TCPSocket(); // client constructor
    explicit TCPSocket(
        std::shared_ptr<SocketAddress> servAddr); // server constructor
    TCPSocket(const TCPSocket &) = delete;
    TCPSocket & operator=(const TCPSocket &) = delete;
    ~TCPSocket() override;

    void setSendFlag(const int);
    void setRecvFlag(const int);

    void listen() override;
    void connect(std::shared_ptr<SocketAddress>);

    // accept and get addr, return connFd
    int accept(std::shared_ptr<SocketAddress>) const;
    int acceptNonBlocking(std::shared_ptr<SocketAddress>) const;
    static int accept(const int socketFd, std::shared_ptr<SocketAddress>);
    static int acceptNonBlocking(const int socketFd,
                                 std::shared_ptr<SocketAddress>);
    std::string recv(const int connFd);                   // server method
    std::string recv();                                   // client method
    void send(const int connFd, const std::string & msg); // server method
    void send(const std::string & msg);                   // client method

private:
    void listenV4();
    void listenV6();
    void connectV4(std::shared_ptr<SocketAddress>);
    void connectV6(std::shared_ptr<SocketAddress>);

    int sendFlag;
    int recvFlag;
};
}
}

#endif /* COLLIE_TCP_TCPSOCKET_H */
