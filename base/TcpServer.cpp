#include "TcpServer.hpp"
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "Socket.hpp"
#include "Channel.hpp"


namespace MiniHttp { namespace Base {

TcpServer::TcpServer(const unsigned port) :
    port(port),
    eventLoop(new EventLoop),
    acceptor(new Acceptor(eventLoop, std::unique_ptr<Socket>(new Socket(port)))) {

    Log(TRACE) << "TcpServer constructing";
}

TcpServer::~TcpServer() {
    Log(TRACE) << "TcpServer destructing";
}

void TcpServer::start() {
    Log(TRACE) << "TcpServer start";
    acceptor->setConnectionCallback(connectCallback);
    acceptor->accept();
    eventLoop->updateChannel(acceptor->getChannel());
    eventLoop->loop();
}

}}
