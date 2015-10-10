#include "TcpServer.hpp"
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "Socket.hpp"
#include "Channel.hpp"


namespace MiniHttp { namespace Base {

TcpServer::TcpServer(const unsigned & port) : port(port) {}

TcpServer::~TcpServer() {}

void TcpServer::listen() {
    eventLoop.reset(new EventLoop);
    acceptor.reset(new Acceptor(eventLoop,
                                std::unique_ptr<Socket>(new Socket(port))));
}

void TcpServer::start() {
    acceptor->accept();
    eventLoop->updateChannel(acceptor->getChannel());
    eventLoop->loop();
}

}}
