#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstring>
#include "HttpServ.hpp"
#include "Request.hpp"
#include "StaticFileHandler.hpp"


namespace Http {
    class Application final {
    public:
        typedef std::function<void(Request)> RequestHandler;
        typedef std::map<std::pair<std::string, Method>, RequestHandler> Router;
        Application();
        Status init(const unsigned&);
        void run();
        void setDefaultTCPHandler();
        std::string generateHeader() const;

    private:
        std::unique_ptr<Socket::TcpSocket> tcpSocket;
        Router router;
        Socket::TcpSocket::Handler tcpHandler;
    };
}


#endif /* APPLICATION_H */
