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
      host(host),
      port(port),
      localAddr(SocketAddress::getSocketAddress(host, port)),
      eventLoop(new Event::EventLoop),
      acceptor(new Acceptor(localAddr, eventLoop)) {
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
    acceptor->accept();
    eventLoop->loop();
}

void
TcpServer::multiThread(const int threadNum) {
    if(threadNum < 1) return;
    isMultiThread = true;
    threadPool.reset(new Event::ThreadPool(threadNum));
}

void
TcpServer::newConnection(const unsigned connFd,
                         std::shared_ptr<SocketAddress> remoteAddr) {
    Log(INFO) << "TcpServer accept fd(" << connFd << ") ip("
              << remoteAddr->getIP() << ")";

    // new channel
    std::shared_ptr<Event::Channel> channel(new Event::Channel(connFd));
    if(isMultiThread) {
        // multi threads
        threadPool->pushChannel(channel);
    } else {
        // single thread
        channel->setEventLoop(this->eventLoop);
    }
    // new connection
    // NOTE setting up channel in connection
    std::shared_ptr<TcpConnection> connection(
        new TcpConnection(channel, this->localAddr, remoteAddr));
    connection->setMessageCallback(onMessageCallback);
    connections.insert(connection); // XXX
    connection->setShutdownCallback(
        [this](std::shared_ptr<TcpConnection> conn) {
            // remove connection
            Log(INFO) << "Connection close";
            this->connections.erase(conn);
        });

    if(connectedCallback) connectedCallback();
}
}
}
