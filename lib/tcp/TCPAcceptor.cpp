#include "../../include/Global.hpp"
#include "../../include/tcp/TCPAcceptor.hpp"
#include "../../include/tcp/TCPSocket.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/InetAddress.hpp"

namespace Collie {
namespace TCP {

TCPAcceptor::TCPAcceptor(SharedPtr<InetAddress> addr) noexcept
    : threadNum(1),
      localAddr(addr),
      tcpSocket(new TCPSocket(localAddr)) {
    Log(TRACE) << "TCPAcceptor constructing";
}

TCPAcceptor::~TCPAcceptor() noexcept {
    Log(TRACE) << "TCPAcceptor destructing";
}

void
TCPAcceptor::bindAndListen() const {
    tcpSocket->bindAndListen();
}

void
TCPAcceptor::setThreadNum(const size_t threadNum) {
    this->threadNum = threadNum;
}

SharedPtr<Event::Channel>
TCPAcceptor::getBaseChannel() {
    REQUIRE_(tcpSocket, "TCP Socket is NULL");

    // create channel
    auto channel = MakeShared<Event::Channel>(tcpSocket);
    // for multi threads it doesn't own the socket fd
    // set callback after setting event loop
    channel->setAfterSetLoopCallback([this](SharedPtr<Event::Channel> channel) {
        Log(TRACE) << "TCPAcceptor channel is setting up";
        channel->enableRead();
        channel->setReadCallback(std::bind(&TCPAcceptor::handleRead, this));
        channel->setErrorCallback(std::bind(&TCPAcceptor::handleError, this));
        channel->enableOneShot(); // NOTE One shot, channel needs to update
                                  // after every accepting
        channel->setUpdateAfterActivate(true);
    });
    return channel;
}

void
TCPAcceptor::handleRead() {
    // should be thread-safe
    auto connSocket = tcpSocket->accept();
    if(connSocket->getState() == TCPSocket::State::IllegalAccept) {
        handleError();
    } else {
        acceptCallback(connSocket);
    }
}

void
TCPAcceptor::handleError() {
    // TODO
    Log(TRACE) << "TCPAcceptor handle error";
}
}
}
