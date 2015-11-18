#include "../../include/Global.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/EventLoopThreadPool.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/tcp/TCPServer.hpp"
#include "../../include/tcp/Acceptor.hpp"
#include "../../include/tcp/TCPSocket.hpp"
#include "../../include/tcp/TCPConnection.hpp"

namespace Collie {
namespace TCP {

TCPServer::TCPServer() : threadNum(1), port(0) {
    Log(TRACE) << "TCPServer constructing";
}

TCPServer::~TCPServer() { Log(TRACE) << "TCPServer destructing"; }

void
TCPServer::bind(const std::string & host, const unsigned port) {
    this->host = host;
    this->port = port;
    localAddr = SocketAddress::getSocketAddress(host, port);
}

void
TCPServer::start() {
    Log(INFO) << "TCPServer start";

    acceptor.reset(new Acceptor(localAddr));
    using namespace std::placeholders;
    // setup acceptor
    acceptor->setAcceptCallback(
        std::bind(&TCPServer::newConnection, this, _1, _2));
    eventLoopThreadPool.reset(new Event::EventLoopThreadPool(threadNum));
    auto acceptChannel = acceptor->getBaseChannel();
    eventLoopThreadPool->startLoop({acceptChannel});
}

void
TCPServer::newConnection(const unsigned connFd,
                         std::shared_ptr<SocketAddress> remoteAddr) {
    Log(INFO) << "TCPServer accept fd(" << connFd << ") ip("
              << remoteAddr->getIP() << ")";

    // new channel
    std::shared_ptr<Event::Channel> channel(new Event::Channel(connFd));
    // NOTE setting up channel in connection
    channel->setAfterSetLoopCallback(
        [this, remoteAddr](std::shared_ptr<Event::Channel> channel) {
            // NOTE only use this->localAddr which should be immutable
            // TCPServer is not thread safe
            // new connection
            auto connection = std::make_shared<TCPConnection>(
                channel, this->localAddr, remoteAddr);
            connection->setMessageCallback(onMessageCallback);
            // store this connection in server
            localThreadConnections.insert(connection);

            connection->setShutdownCallback(
                [](std::shared_ptr<TCPConnection> conn) {
                    // remove this connection from server
                    Log(INFO) << "Connection close";
                    localThreadConnections.erase(conn);
                });
        });
    eventLoopThreadPool->pushChannel(channel);

    if(connectedCallback) connectedCallback();
}
}
}
