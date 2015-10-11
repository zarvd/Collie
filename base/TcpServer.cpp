#include "TcpServer.hpp"
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "Socket.hpp"
#include "Channel.hpp"


namespace MiniHttp { namespace Base {

TcpServer::TcpServer(const unsigned & port) : port(port) {}

TcpServer::~TcpServer() {}

TcpServer & TcpServer::listen() {
    eventLoop.reset(new EventLoop);
    acceptor.reset(new Acceptor(eventLoop,
                                std::unique_ptr<Socket>(new Socket(port))));
    acceptor->accept();
    return * this;
}

void TcpServer::start() {
    eventLoop->loop();
}

}}
