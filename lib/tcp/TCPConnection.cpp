#include "../../include/Global.hpp"
#include "../../include/tcp/TCPConnection.hpp"
#include "../../include/Socket.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include <functional>
#include <algorithm>
#include <future>

namespace Collie {
namespace TCP {

TCPConnection::TCPConnection(std::shared_ptr<Event::Channel> channel,
                             std::shared_ptr<SocketAddress> localAddr,
                             std::shared_ptr<SocketAddress> remoteAddr)
    : connected(true),
      channel(channel),
      localAddr(localAddr),
      remoteAddr(remoteAddr) {

    REQUIRE(channel);
    // set channel callback and enable reading
    channel->setReadCallback(std::bind(&TCPConnection::handleRead, this));
    channel->setWriteCallback(std::bind(&TCPConnection::handleWrite, this));
    channel->setCloseCallback(std::bind(&TCPConnection::handleClose, this));
    channel->setErrorCallback(std::bind(&TCPConnection::handleError, this));
    channel->enableRead();
    channel->enableWrite();
    Log(TRACE) << "TCP Connection is constructing";
}

TCPConnection::~TCPConnection() {
    Log(TRACE) << "TCP Connection is destructing";
}

void
TCPConnection::disconnect() {
    Log(DEBUG) << "TCP Connection is trying to disconnect";
    connected = false;
    if(outputBuffer.empty()) shutdown();
}

void
TCPConnection::shutdown() {
    Log(TRACE) << "TCP Connection is shutting down";
    channel->remove();
    if(shutdownCallback) shutdownCallback(shared_from_this());
}

std::string
TCPConnection::recvAll() {
    // FIXME
    std::string buffer;
    std::swap(buffer, inputBuffer);
    return buffer;
}

void
TCPConnection::send(const std::string & buffer) {
    // FIXME
    outputBuffer += buffer;
    if(!channel->isWrite()) channel->enableWrite();
}

void
TCPConnection::handleRead() {
    std::string content;
    const auto size = Socket::recv(channel->getFd(), content);
    if(size > 0) {
        inputBuffer += content;
        messageCallback(shared_from_this());
    } else {
        handleClose();
    }
    channel->disableRead();
    if(!outputBuffer.empty()) {
        if(!channel->isWrite()) channel->enableWrite();
    } else if(!connected) {
        shutdown();
    }
}

void
TCPConnection::handleWrite() {
    if(!outputBuffer.empty()) {
        const auto size = Socket::send(channel->getFd(), outputBuffer);
        const int bufferSize = outputBuffer.length();
        if(bufferSize == size - 1) {
            outputBuffer.clear();
        } else if(size > 0) {
            outputBuffer = outputBuffer.substr(size - 1);
        }
    }
    channel->disableWrite();
    channel->enableRead();
    if(!connected) shutdown();
}

void
TCPConnection::handleClose() {
    disconnect();
}

void
TCPConnection::handleError() {
    shutdown();
}
}
}
