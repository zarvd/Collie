#ifndef MINIHTTP_HTTP_APPLICATION_HPP
#define MINIHTTP_HTTP_APPLICATION_HPP

#include <cstring>

#include "Request.hpp"
#include "../base/TcpSocket.hpp"


namespace MiniHttp { namespace Http {

class Application {
public:
    typedef std::function<void(Request)> RequestHandler;
    typedef std::map<std::pair<std::string, Method>, RequestHandler> Router;

    Application();
    Application(const Application &) = delete;
    Application operator=(const Application &) = delete;
    ~Application();

    Status init(const unsigned&);
    void run();
    void setDefaultTCPHandler();
    std::string generateHeader() const;

private:

    std::unique_ptr<Socket::TcpSocket> tcpSocket;
    Router router;
    Socket::TcpSocket::Handler tcpHandler;
};

}}

#endif /* MINIHTTP_HTTP_APPLICATION_HPP */
