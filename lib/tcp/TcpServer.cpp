#include "../../include/Global.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/tcp/TcpServer.hpp"
#include "../../include/tcp/Acceptor.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/tcp/TcpConnection.hpp"


namespace Collie { namespace Tcp {

TcpServer::TcpServer(const std::string & host,
                     const unsigned & port) :
    host(host),
    port(port),
    localAddr(SocketAddress::getSocketAddress(host, port)),
    eventLoop(new Event::EventLoop),
    acceptor(new Acceptor(localAddr, eventLoop)) {
    Log(TRACE) << "TcpServer constructing";
}

TcpServer::~TcpServer() {
    Log(TRACE) << "TcpServer destructing";
}

void
TcpServer::start() {
    Log(TRACE) << "TcpServer start";

    using namespace std::placeholders;
    // setup acceptor
    acceptor->setAcceptCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
    acceptor->accept();
    eventLoop->updateChannel(acceptor->getChannel());
    eventLoop->loop();
}

void
TcpServer::newConnection(const unsigned & connFd, std::shared_ptr<SocketAddress> remoteAddr) {
    Log(INFO) << "TcpServer accept fd(" << connFd << ") ip(" << remoteAddr->getIP() << ")";

    // new connection
    std::shared_ptr<TcpConnection> connection(new TcpConnection(this->eventLoop, connFd, this->localAddr, remoteAddr));
    connection->setMessageCallback(onMessageCallback);
    clients.push_back(connection);  // FIXME

    // user callback
    if(connectedCallback) connectedCallback();
}

}}
