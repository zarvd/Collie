#ifndef COLLIE_NETWORK_TCP_TCPSOCKET_H
#define COLLIE_NETWORK_TCP_TCPSOCKET_H

#include "../Socket.hpp"


namespace Collie { namespace Network { namespace Tcp {

class TcpSocket : public Socket {
public:
    TcpSocket(const unsigned & port,
              std::shared_ptr<SocketAddress>);
    TcpSocket(const TcpSocket &) = delete;
    TcpSocket operator=(const TcpSocket &) = delete;
    ~TcpSocket() override;

    void listen() override;

    // accept and get addr, return connFd
    int accept(std::shared_ptr<SocketAddress>) const;
    static int accept(const int & socketFd, std::shared_ptr<SocketAddress>);
    static std::string recv(const int & connFd);
    static void send(const int & connFd, const std::string & msg);

private:
    void listenV4();
    void listenV6();

    int sendFlag;
    int recvFlag;
};

}}}



#endif /* COLLIE_NETWORK_TCP_TCPSOCKET_H */
