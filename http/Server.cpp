#include "Server.hpp"
#include "StaticFileHandler.hpp"


namespace MiniHttp { namespace Http {

Server::Server() :
    tcpSocket(nullptr),
    tcpHandler(nullptr) {

    Log(DEBUG) << "HTTP handler created";
    setDefaultTCPHandler();
}

Server::~Server() {}

void Server::setDefaultTCPHandler() {
    // init default tcp handler
    tcpHandler = [this](const int& connFd) -> void {
        // FIXME parse header and set route
        // BUG readline and detective if it is a new request
        // TODO maximum header size
        char header[8192];
        recv(connFd, header, 8192, 0);
        Request req = Request::parse(header);

        StaticFileHandler staticHandler("./", "/");
        Response res = staticHandler.view(req);

        char page[res.get().length() + 1];
        std::strcpy(page, res.get().c_str());
        send(connFd, page, sizeof(page), 0);
    };
}

/**
 * Initialize a HTTP handler before it runs
 */
Status Server::init(const unsigned int& port) {
    Log(DEBUG) << "HTTP handler trying to init";
    try {
        tcpSocket = std::unique_ptr<Socket::TcpSocket>(new Socket::TcpSocket);
        tcpSocket->init(port, IP::IPv4);
        tcpSocket->setConnectHandler(tcpHandler);
        Log(DEBUG) << "HTTP handler inited";
        return Status::SUCCESS;
    } catch(const std::exception& err) {
        Log(ERROR) << "HTTP handler fail to init";
        Log(ERROR) << err.what();
        return Status::FAIL;
    }
}

/**
 *
 */
void Server::run() {
    try {
        Log(INFO) << "HTTP Server running on port " << tcpSocket->getPort();
        tcpSocket->run();
    } catch(const std::exception& err) {
        Log(ERROR) << "HTTP handler fail to run";
        Log(ERROR) << err.what();
    }
}

std::string Server::generateHeader() const {
    return "";
}

}}
