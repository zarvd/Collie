#include "TcpServer.hpp"
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "Socket.hpp"


namespace MiniHttp { namespace Base {

TcpServer::TcpServer(const unsigned & port) : port(port) {}

TcpServer::~TcpServer() {}

void TcpServer::listen() {
    Socket socket(port);
    eventLoop = std::shared_ptr<EventLoop>(new EventLoop);
    acceptor = std::unique_ptr<Acceptor>(new Acceptor(eventLoop, socket.getFd()));

}

void TcpServer::start() {
}

}}
