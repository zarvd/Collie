#include "TcpServer.hpp"
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "Socket.hpp"
#include "Channel.hpp"


namespace MiniHttp { namespace Base {

TcpServer::TcpServer(const unsigned & port) : port(port) {}

TcpServer::~TcpServer() {}

void TcpServer::listen() {
    Socket socket(port);
    eventLoop.reset(new EventLoop);
    acceptor.reset(new Acceptor(eventLoop, socket.getFd()));
    eventLoop->updateChannel(acceptor->getChannel());
}

void TcpServer::start() {
}

}}
