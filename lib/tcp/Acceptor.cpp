#include "../../include/Global.hpp"
#include "../../include/tcp/Acceptor.hpp"
#include "../../include/tcp/TCPSocket.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/InetAddress.hpp"

namespace Collie {
namespace TCP {

Acceptor::Acceptor(std::shared_ptr<InetAddress> addr) noexcept
    : threadNum(1),
      localAddr(addr),
      tcpSocket(new TCPSocket(localAddr)) {
    tcpSocket->bindAndListen();
    Log(TRACE) << "Acceptor constructing";
}

Acceptor::~Acceptor() noexcept { Log(TRACE) << "Acceptor destructing"; }

void
Acceptor::setThreadNum(const size_t threadNum) {
    this->threadNum = threadNum;
}

SharedPtr<Event::Channel>
Acceptor::getBaseChannel() {
    REQUIRE_(tcpSocket, "TCP Socket is NULL");

    // create channel
    auto channel = MakeShared<Event::Channel>(tcpSocket);
    // for multi threads it doesn't own the socket fd
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
    auto connSocket = tcpSocket->accept();
    if(connSocket->getState() == TCPSocket::State::IllegalAccept) {
        handleError();
    } else {
        acceptCallback(connSocket);
    }
}

void
Acceptor::handleError() {
    // TODO
    Log(TRACE) << "Acceptor handle error";
}
}
}
