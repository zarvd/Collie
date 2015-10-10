#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "Socket.hpp"
#include "Channel.hpp"


namespace MiniHttp { namespace Base {

Acceptor::Acceptor(std::shared_ptr<EventLoop> eventLoop, const int& socketFd) :
    socketFd(socketFd),
    eventLoop(eventLoop),
    channel(new Channel(eventLoop, socketFd)){

    channel->setReadCallback(std::bind(&Acceptor::connectCallback, this));
}

Acceptor::~Acceptor() {}

void Acceptor::accept() {
}

}}
