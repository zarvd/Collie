#include "../../include/Global.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/ThreadPool.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/tcp/TcpServer.hpp"
#include "../../include/tcp/Acceptor.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/tcp/TcpConnection.hpp"

namespace Collie {
namespace Tcp {

TcpServer::TcpServer(const std::string & host, const unsigned port)
    : isMultiThread(false),
      threadNum(0),
      host(host),
      port(port),
      localAddr(SocketAddress::getSocketAddress(host, port)),
      acceptor(new Acceptor(localAddr)) {
    Log(TRACE) << "TcpServer constructing";
}

TcpServer::~TcpServer() { Log(TRACE) << "TcpServer destructing"; }

void
TcpServer::start() {
    Log(INFO) << "TcpServer start";

    using namespace std::placeholders;
    // setup acceptor
    acceptor->setAcceptCallback(
        std::bind(&TcpServer::newConnection, this, _1, _2));
    if(!isMultiThread) {
        eventLoop.reset(new Event::EventLoop);
        auto channel = acceptor->getBaseChannel();
        channel->setEventLoop(eventLoop);
        channel->update();
        eventLoop->loop();
    } else {
        threadPool.reset(new Event::ThreadPool(threadNum));
        threadPool->start(acceptor->getBaseChannel());
    }
}

void
TcpServer::multiThread(const int threadNum) {
    if(threadNum < 1) {
        isMultiThread = false;
    } else {
        isMultiThread = true;
        this->threadNum = threadNum;
    }
}

void
TcpServer::newConnection(const unsigned connFd,
                         std::shared_ptr<SocketAddress> remoteAddr) {
    Log(INFO) << "TcpServer accept fd(" << connFd << ") ip("
              << remoteAddr->getIP() << ")";

    // new channel
    std::shared_ptr<Event::Channel> channel(new Event::Channel(connFd));
    // new connection
    // NOTE setting up channel in connection
    channel->setAfterSetLoopCallback(
        [this, remoteAddr](std::shared_ptr<Event::Channel> channel) {
            std::shared_ptr<TcpConnection> connection(
                new TcpConnection(channel, this->localAddr, remoteAddr));
            connection->setMessageCallback(onMessageCallback);
            this->connections.insert(connection); // XXX
            connection->setShutdownCallback(
                [this](std::shared_ptr<TcpConnection> conn) {
                    // remove connection
                    Log(INFO) << "Connection close";
                    this->connections.erase(conn);
                });
        });
    if(isMultiThread) {
        // multi threads
        threadPool->pushChannel(channel);
    } else {
        // single thread
        channel->setEventLoop(this->eventLoop);
    }
    if(connectedCallback) connectedCallback();
}
}
}
