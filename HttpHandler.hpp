#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include "HttpServ.hpp"
#include "HttpRequest.hpp"


namespace Http {
    class HttpHandler final {
    private:
        std::unique_ptr<Socket::TcpSocket> tcpSocket;

    public:
        std::map<std::string, bool> router;
        Socket::TcpSocket::Handler tcpHandler;

        HttpHandler();
        Status init(const unsigned&);
        void run();
        void setDefaultTCPHandler();
        std::string generateHeader() const;
    };
}

#endif /* HTTPHANDLER_H */
