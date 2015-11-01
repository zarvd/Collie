#ifndef COLLIE_TCP_TCPSOCKET_H
#define COLLIE_TCP_TCPSOCKET_H

#include "../Socket.hpp"


namespace Collie { namespace Tcp {

class TcpSocket : public Socket {
public:
    TcpSocket();  // client constructor
    explicit TcpSocket(std::shared_ptr<SocketAddress> servAddr);  // server constructor
    TcpSocket(const TcpSocket &) = delete;
    TcpSocket & operator=(const TcpSocket &) = delete;
    ~TcpSocket() override;

    void setSendFlag(const int);
    void setRecvFlag(const int);

    void listen() override;
    void connect(std::shared_ptr<SocketAddress>) override;

    // accept and get addr, return connFd
    // NOTE it will block, but we don't have to worry about it because of the lib is based on poller and the timer issue will be avoided in event loop
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

}}



#endif /* COLLIE_TCP_TCPSOCKET_H */
