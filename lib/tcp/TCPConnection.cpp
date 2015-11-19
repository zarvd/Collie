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

thread_local std::unordered_set<std::shared_ptr<TCPConnection>>
    localThreadConnections;

TCPConnection::TCPConnection(std::shared_ptr<Event::Channel> channel,
                             std::shared_ptr<const SocketAddress> localAddr,
                             std::shared_ptr<const SocketAddress> remoteAddr)
    : connected(true),
      isShutDown(false),
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
    if(isShutDown) {
        Log(DEBUG) << "TCP Connection was already shut down";
        return;
    }
    isShutDown = true;
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
TCPConnection::sendFile(const std::string & fileName) {
    REQUIRE(channel);
    Socket::sendFile(channel->getFd(), fileName);
}

void
TCPConnection::recvFile(const std::string & fileName, const size_t fileSize) {
    REQUIRE(channel);
    Socket::recvFile(channel->getFd(), fileName, fileSize);
}

void
TCPConnection::handleRead() {
    std::string content;
    // Non blocking receiving
    const auto size = Socket::recv(channel->getFd(), content, MSG_DONTWAIT);
    if(size > 0) {
        inputBuffer += content;
        messageCallback(shared_from_this());
        channel->disableRead();
        if(!outputBuffer.empty()) {
            if(!channel->isWrite()) channel->enableWrite();
        } else if(!connected) {
            shutdown();
        }
    } else if(size == 0) {
        handleClose();
    } else {
        handleError();
    }
}

void
TCPConnection::handleWrite() {
    // XXXX
    if(!outputBuffer.empty()) {
        const ssize_t size =
            Socket::send(channel->getFd(), outputBuffer, MSG_DONTWAIT);
        // const ssize_t bufferSize = outputBuffer.length();
        // if(bufferSize == size - 1) {
        //     outputBuffer.clear();
        // } else if(size > 0) {
        //     outputBuffer = outputBuffer.substr(size - 1);
        // }
        if(size > 0) outputBuffer.clear();
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
