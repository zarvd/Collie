#include "Acceptor.hpp"
#include "EventLoop.hpp"
#include "Socket.hpp"
#include "Channel.hpp"


namespace MiniHttp { namespace Base {

Acceptor::Acceptor(std::shared_ptr<EventLoop> eventLoop, std::unique_ptr<Socket> socket) :
    socket(std::move(socket)),  // be careful about the parameter SOCKET, it is not available anymore
    eventLoop(eventLoop),
    channel(new Channel(eventLoop, this->socket->getFd())) {
}

Acceptor::~Acceptor() {}

void Acceptor::accept() const {
    channel->enableRead();
    channel->setReadCallback([this](const int &) {
            std::shared_ptr<SocketAddress> addr;
            int connFd = socket->accept(addr);
            if(connFd > 0) {
                connectCallback(connFd);
                // std::shared_ptr<Channel> connChannel(new Channel(eventLoop, connFd));
                // TODO setup the connection callback of this channel
                // connChannel->setReadCallback([](const int & fd) {
                //         const std::string msg = Socket::recv(fd);

                //     });
                // eventLoop->updateChannel(connChannel);
            }
        });
}

}}
