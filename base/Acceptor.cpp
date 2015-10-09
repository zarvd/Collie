#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "Socket.hpp"


namespace MiniHttp { namespace Base {

Acceptor::Acceptor(std::shared_ptr<EventLoop> eventLoop, const int& socketFd) :
    socketFd(socketFd),
    eventLoop(eventLoop) {}

Acceptor::~Acceptor() {}

void Acceptor::accept() {
}

}}
