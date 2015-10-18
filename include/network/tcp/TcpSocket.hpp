#ifndef COLLIE_NETWORK_TCP_TCPSOCKET_H
#define COLLIE_NETWORK_TCP_TCPSOCKET_H

#include "../Socket.hpp"


namespace Collie { namespace Network { namespace Tcp {

class TcpSocket : public Socket {
public:
    TcpSocket();  // client constructor
    explicit TcpSocket(std::shared_ptr<SocketAddress> servAddr);  // server constructor
    TcpSocket(const TcpSocket &) = delete;
    TcpSocket operator=(const TcpSocket &) = delete;
    ~TcpSocket() override;

    void listen() override;
    void connect(std::shared_ptr<SocketAddress>) override;

    // accept and get addr, return connFd
    int accept(std::shared_ptr<SocketAddress>) const;
    static int accept(const int & socketFd, std::shared_ptr<SocketAddress>);
    std::string recv(const int & connFd);
    static std::string recv(const int & connFd, const int & recvFlag);
    void send(const int & connFd, const std::string & msg);
    static void send(const int & connFd, const std::string & msg, const int & sendFlag);

private:
    void listenV4();
    void listenV6();
    void connectV4(std::shared_ptr<SocketAddress>);
    void connectV6(std::shared_ptr<SocketAddress>);

    int sendFlag;
    int recvFlag;
};

}}}



#endif /* COLLIE_NETWORK_TCP_TCPSOCKET_H */
