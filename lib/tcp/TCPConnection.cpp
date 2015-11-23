#include "../../include/Global.hpp"
#include "../../include/Socket.hpp"
#include "../../include/InetAddress.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/tcp/TCPSocket.hpp"
#include "../../include/tcp/TCPConnection.hpp"
#include "../../include/utils/File.hpp"

namespace Collie {
namespace TCP {

thread_local std::unordered_set<SharedPtr<TCPConnection>>
    localThreadConnections;

TCPConnection::TCPConnection(SharedPtr<Event::Channel> channel)
    : connected(true), isShutDown(false), channel(channel) {

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

SharedPtr<InetAddress>
TCPConnection::getRemoteAddress() const {
    auto fd = channel->getDescriptor();
    auto socket = std::dynamic_pointer_cast<TCPSocket>(fd);
    return socket->getAddress();
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

String
TCPConnection::recvAll() {
    // FIXME
    String buffer;
    std::swap(buffer, inputBuffer);
    return buffer;
}

void
TCPConnection::send(const String & buffer) {
    // FIXME
    outputBuffer += buffer;
    if(!channel->isWrite()) channel->enableWrite();
}

void
TCPConnection::sendFile(const String & fileName) {
    REQUIRE(channel);
    Utils::File file(fileName, Utils::File::Mode::Read);
    if(file.isExisted() && file.isFile()) {
        Socket::sendFile(channel->getDescriptor(), file);
    } else {
        // XXX
        Log(WARN) << fileName << " not found";
    }
}

void
TCPConnection::sendFile(const Utils::File & file) {
    if(file.isExisted() && file.isFile()) {
        Socket::sendFile(channel->getDescriptor(), file);
    } else {
        // XXX
        Log(WARN) << file.getName() << " not found";
    }
}

void
TCPConnection::recvFile(const String & fileName, const size_t fileSize) {
    REQUIRE(channel);
    Utils::File file(fileName, Utils::File::Mode::Write);
    Socket::recvFile(channel->getDescriptor(), file, fileSize);
}

void
TCPConnection::handleRead() {
    String content;
    // Non blocking receiving
    const auto size =
        Socket::recv(channel->getDescriptor(), content, MSG_DONTWAIT);
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
            Socket::send(channel->getDescriptor(), outputBuffer, MSG_DONTWAIT);
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
