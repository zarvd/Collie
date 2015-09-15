#ifndef SOCKET_H
#define SOCKET_H

#include "httpd.hpp"


namespace Socket {
    struct TcpHandler {
        Status (*handler)(const int);
    };

    class TcpSocket final {
    public:
        TcpSocket();
        ~TcpSocket();

        IP getIPVersion() const;
        Status init(const unsigned int&, const IP&);
        Status run();
        Status setHandler(std::shared_ptr<TcpHandler>);

    private:
        Status initIPv4(const unsigned int&);
        Status initIPv6(const unsigned int&);
        IP ipVersion;  // IP version
        int socketFd;  // listening socket descriptor
        std::shared_ptr<TcpHandler> handler = nullptr;
    };
}

#endif /* SOCKET_H */
