#include "../../include/tcp/TcpConnection.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/event/Channel.hpp"
#include <functional>


namespace Collie { namespace Tcp {

TcpConnection::TcpConnection(std::shared_ptr<Event::Channel> channel,
                             std::shared_ptr<SocketAddress> localAddr,
                             std::shared_ptr<SocketAddress> remoteAddr) :
    channel(channel),
    localAddr(localAddr),
    remoteAddr(remoteAddr) {

    channel->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    channel->enableRead();
    channel->enableWrite();
}

TcpConnection::~TcpConnection() {
}

void TcpConnection::disconnect() {
}

std::string TcpConnection::recvAll() {
    const std::string content;
    return content;
}

void TcpConnection::send(const std::string &) {
    // append to output buffer
}

void TcpConnection::handleRead() {
    const std::string content = TcpSocket::recv(channel->getFd(), 0);
    if(content.size() > 0) {
        inputBuffer += content;
    } else {
        channel->disableRead();
    }
}

void TcpConnection::handleWrite() {

}

void TcpConnection::handleClose() {
}

void TcpConnection::handleError() {
}

}}
