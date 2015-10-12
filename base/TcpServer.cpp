#include "TcpServer.hpp"
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "Socket.hpp"
#include "Channel.hpp"
#include "TcpConnection.hpp"


namespace MiniHttp { namespace Base {

TcpServer::TcpServer(const unsigned port) :
    port(port),
    eventLoop(new EventLoop),
    acceptor(new Acceptor(eventLoop, std::unique_ptr<Socket>(new Socket(port)))) {

    Log(TRACE) << "TcpServer constructing";
}

TcpServer::~TcpServer() {
    Log(TRACE) << "TcpServer destructing";
}

void TcpServer::start() {
    Log(TRACE) << "TcpServer start";

    // setup acceptor
    acceptor->setAcceptCallback([this](const unsigned connFd, std::shared_ptr<SocketAddress> addr) {
            Log(INFO) << "TcpServer accept fd(" << connFd << ") ip(" << addr->getIP() << ")";

            // new connection channel
            std::shared_ptr<Channel> connChannel(new Channel(this->eventLoop, connFd));
            // add this channel to event loop
            this->eventLoop->updateChannel(connChannel);

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
