#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "httpd.hpp"


namespace Socket {
    class TcpSocket final {
    public:
        typedef std::function<void(const int&)> Handler;
        TcpSocket();
        ~TcpSocket();

        Status init(const unsigned&, const IP&);
        Status run();

        Status setConnectHandler(Handler);

        IP getIPVersion() const;
        unsigned getPort() const;

    private:
        IP ipVersion;  // IP version
        int listenFd;  // listening socket descriptor
        unsigned port;

        Handler connectHandler;

        Status initIPv4(const unsigned&);
        Status initIPv6(const unsigned&);
    };
}

#endif /* SOCKET_H */
