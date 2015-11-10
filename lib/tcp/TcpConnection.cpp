#include "../../include/Global.hpp"
#include "../../include/tcp/TcpConnection.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include <functional>
#include <algorithm>
#include <future>

namespace Collie {
namespace Tcp {

TcpConnection::TcpConnection(std::shared_ptr<Event::Channel> channel,
                             std::shared_ptr<SocketAddress> localAddr,
                             std::shared_ptr<SocketAddress> remoteAddr)
    : connected(true),
      channel(channel),
      localAddr(localAddr),
      remoteAddr(remoteAddr) {

    // set channel callback and enable reading
    channel->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    channel->enableRead();
    channel->enableWrite();
    Log(TRACE) << "Tcp Connection constructing";
}

TcpConnection::~TcpConnection() { Log(TRACE) << "Tcp Connection destructing"; }

void
TcpConnection::disconnect() {
    connected = false;
    if(outputBuffer.empty()) shutdown();
}

void
TcpConnection::shutdown() {
    channel->remove();
    if(shutdownCallback) shutdownCallback(shared_from_this());
}

std::string
TcpConnection::recvAll() {
    // FIXME
    std::string buffer;
    std::swap(buffer, inputBuffer);
    return buffer;
}

void
TcpConnection::send(const std::string & buffer) {
    // FIXME
    outputBuffer += buffer;
    channel->enableWrite();
}

void
TcpConnection::handleRead() {
    const auto content = TcpSocket::recv(channel->getFd(), 0);
    if(!content.empty()) {
        inputBuffer += content;
        messageCallback(shared_from_this());
        channel->disableRead();
    } else {
        handleClose();
    }
    if(!connected && outputBuffer.empty()) shutdown();
}

void
TcpConnection::handleWrite() {
    if(!outputBuffer.empty()) {
        // FIXME slice outputBuffer
        TcpSocket::send(channel->getFd(), outputBuffer, 0);
        outputBuffer.clear();
    }
    channel->disableWrite();
    channel->enableRead();
    if(!connected) shutdown();
}

void
TcpConnection::handleClose() {
    disconnect();
}

void
TcpConnection::handleError() {
    disconnect();
}
}
}
