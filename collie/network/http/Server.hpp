#ifndef MINIHTTP_HTTP_SERVER_HPP
#define MINIHTTP_HTTP_SERVER_HPP

#include <cstring>

#include "Request.hpp"
#include "../base/TcpSocket.hpp"


namespace MiniHttp { namespace Http {

class Server {
public:
    typedef std::function<void(Request)> RequestHandler;
    typedef std::map<std::pair<std::string, Method>, RequestHandler> Router;

    Server();
    Server(const Server &) = delete;
    Server operator=(const Server &) = delete;
    ~Server();

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

#endif /* MINIHTTP_HTTP_SERVER_HPP */
