#include "../../include/Global.hpp"
#include "../../include/tcp/Acceptor.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/SocketAddress.hpp"

namespace Collie {
namespace Tcp {

Acceptor::Acceptor(std::shared_ptr<SocketAddress> addr)
    : threadNum(1), localAddr(addr) {
    Log(TRACE) << "Acceptor constructing";
}

Acceptor::~Acceptor() { Log(TRACE) << "Acceptor destructing"; }

void
Acceptor::setThreadNum(const size_t threadNum) {
    this->threadNum = threadNum;
}

int
Acceptor::getSocketFd() const {
    return tcpSocket->getFd();
}

void
Acceptor::socket() {
    // create socket and listen
    tcpSocket.reset(new TcpSocket(localAddr));
    tcpSocket->listen();
}

std::shared_ptr<Event::Channel>
Acceptor::getBaseChannel() {
    if(!tcpSocket) socket();

    // create channel
    auto channel = std::make_shared<Event::Channel>(tcpSocket->getFd());
    // for multi threads it doesn't own the socket fd
    channel->setIsOwnFd(false);
    // set callback after setting event loop
    channel->setAfterSetLoopCallback(
        [this](std::shared_ptr<Event::Channel> channel) {
            Log(TRACE) << "Acceptor channel is setting up";
            channel->enableRead();
            channel->setReadCallback(std::bind(&Acceptor::handleRead, this));
            channel->setErrorCallback(std::bind(&Acceptor::handleError, this));
            channel->enableOneShot(); // NOTE One shot, channel needs to update
                                      // after every accepting
            channel->setUpdateAfterActivate(true);
        });
    return channel;
}

void
Acceptor::handleRead() {
    // should be thread-safe
    auto remoteAddr = std::make_shared<SocketAddress>();
    int connFd = tcpSocket->accept(remoteAddr);
    if(connFd > 0) {
        acceptCallback(connFd, remoteAddr);
    } else {
        handleError();
    }
}

void
Acceptor::handleError() {
    // TODO
    Log(TRACE) << "Acceptor handle error";
}
}
}
