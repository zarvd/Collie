#include "../../../include/network/tcp/Acceptor.hpp"
#include "../../../include/event/EventLoop.hpp"
#include "../../../include/network/tcp/TcpSocket.hpp"
#include "../../../include/event/Channel.hpp"
#include "../../../include/network/SocketAddress.hpp"


namespace Collie { namespace Network { namespace Tcp {

Acceptor::Acceptor(std::shared_ptr<SocketAddress> addr,
                   std::shared_ptr<Event::EventLoop> eventLoop) :
    localAddr(addr),
    eventLoop(eventLoop) {}

Acceptor::~Acceptor() {}

void
Acceptor::socket() {
    tcpSocket.reset(new TcpSocket);
    channel.reset(new Event::Channel(eventLoop, tcpSocket->getFd()));
    tcpSocket->listen();
}

void
Acceptor::accept() {
    if( ! channel) socket();
    channel->enableRead();
    channel->setReadCallback([this](const unsigned) {
            std::shared_ptr<SocketAddress> remoteAddr(new SocketAddress);
            int connFd = tcpSocket->accept(remoteAddr);
            if(connFd > 0) {
                acceptCallback(connFd, remoteAddr);
            }
        });
}

}}}
