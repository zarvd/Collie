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

    // setup acceptor
    acceptor->setAcceptCallback([this](const unsigned & connFd,
                                       std::shared_ptr<SocketAddress> remoteAddr) {
            Log(INFO) << "TcpServer accept fd(" << connFd << ") ip(" << remoteAddr->getIP() << ")";

            // new connection channel
            std::shared_ptr<Event::Channel> connChannel(new Event::Channel(this->eventLoop, connFd));
            // add this channel to event loop
            this->eventLoop->updateChannel(connChannel);

            // read event
            connChannel->setReadCallback([this, connChannel](const unsigned fd) {
                    std::shared_ptr<TcpConnection> connection(new TcpConnection());
                    this->connectCallback(connection);
                });

            // close event
            connChannel->setCloseCallback([connChannel](const unsigned fd) {
                    Log(INFO) << "Connection " << fd << " is closed";
                    connChannel->remove();
                });

            // error event
            connChannel->setErrorCallback([connChannel](const unsigned fd) {
                    Log(INFO) << "Connection " << fd << " meets error";
                    connChannel->remove();
                });

        });
    acceptor->accept();
    eventLoop->updateChannel(acceptor->getChannel());
    eventLoop->loop();
}

}}
