#ifndef SOCKET_H
#define SOCKET_H

#include "httpd.hpp"


namespace Socket {
    class TcpSocket final {
    public:
        typedef std::function<void(const int&)> Handler;
        TcpSocket();
        ~TcpSocket();

        IP getIPVersion() const;
        Status init(const unsigned&, const IP&);
        Status run();
        Status setConnectHandler(Handler);

    private:
        Status initIPv4(const unsigned&);
        Status initIPv6(const unsigned&);
        IP ipVersion;  // IP version
        int listenFd;  // listening socket descriptor
        Handler connectHandler;
    };
}

#endif /* SOCKET_H */
