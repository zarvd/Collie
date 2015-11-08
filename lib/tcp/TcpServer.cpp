#include "../../include/Global.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/EventLoopThreadPool.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/tcp/TcpServer.hpp"
#include "../../include/tcp/Acceptor.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/tcp/TcpConnection.hpp"

namespace Collie {
namespace Tcp {

TcpServer::TcpServer() : threadNum(1), port(0) {
    Log(TRACE) << "TcpServer constructing";
}

TcpServer::~TcpServer() { Log(TRACE) << "TcpServer destructing"; }

void
TcpServer::bind(const std::string & host, const unsigned port) {
    this->host = host;
    this->port = port;
    localAddr = SocketAddress::getSocketAddress(host, port);
}

void
TcpServer::start() {
    Log(INFO) << "TcpServer start";

    acceptor.reset(new Acceptor(localAddr));
    using namespace std::placeholders;
    // setup acceptor
    acceptor->setAcceptCallback(
        std::bind(&TcpServer::newConnection, this, _1, _2));
    eventLoopThreadPool.reset(new Event::EventLoopThreadPool(threadNum));
    auto acceptChannel = acceptor->getBaseChannel();
    eventLoopThreadPool->startLoop({acceptChannel});
}

void
TcpServer::newConnection(const unsigned connFd,
                         std::shared_ptr<SocketAddress> remoteAddr) {
    Log(INFO) << "TcpServer accept fd(" << connFd << ") ip("
              << remoteAddr->getIP() << ")";

    // new channel
    std::shared_ptr<Event::Channel> channel(new Event::Channel(connFd));
    // NOTE setting up channel in connection
    channel->setAfterSetLoopCallback(
        [this, remoteAddr](std::shared_ptr<Event::Channel> channel) {
            // new connection
            auto connection = std::make_shared<TcpConnection>(
                channel, this->localAddr, remoteAddr);
            connection->setMessageCallback(onMessageCallback);
            // store this connection in server
            this->connections.insert(connection);

            connection->setShutdownCallback(
                [this](std::shared_ptr<TcpConnection> conn) {
                    // remove this connection from server
                    Log(INFO) << "Connection close";
                    this->connections.erase(conn);
                });
        });
    eventLoopThreadPool->pushChannel(channel);

    if(connectedCallback) connectedCallback();
}
}
}
