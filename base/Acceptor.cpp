#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "Socket.hpp"
#include "Channel.hpp"


namespace MiniHttp { namespace Base {

Acceptor::Acceptor(std::shared_ptr<EventLoop> eventLoop, std::unique_ptr<Socket> socket) :
    socket(std::move(socket)),  // be careful about the parameter SOCKET, it is not available anymore
    eventLoop(eventLoop),
    channel(new Channel(eventLoop, this->socket->getFd())) {

    this->socket->listen();
}

Acceptor::~Acceptor() {}

void Acceptor::accept() const {
    channel->enableRead();
    channel->setReadCallback([this](const int) {
            std::shared_ptr<SocketAddress> addr(new SocketAddress);
            int connFd = socket->accept(addr);
            if(connFd > 0) {
                connectCallback(connFd);
            }
        });
}

}}
