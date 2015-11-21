#include "../../include/Global.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/EventLoopThreadPool.hpp"
#include "../../include/InetAddress.hpp"
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
TCPServer::bind(const String & host, const unsigned port) {
    this->host = host;
    this->port = port;
    localAddr = InetAddress::getInetAddress(host, port);
}

void
TCPServer::start() {
    Log(INFO) << "TCPServer start";

    acceptor.reset(new Acceptor(localAddr));
    using namespace std::placeholders;
    // setup acceptor
    acceptor->setAcceptCallback(std::bind(&TCPServer::newConnection, this, _1));
    eventLoopThreadPool.reset(new Event::EventLoopThreadPool(threadNum));
    auto acceptChannel = acceptor->getBaseChannel();
    eventLoopThreadPool->startLoop({acceptChannel});
}

void
TCPServer::newConnection(SharedPtr<TCPSocket> connSocket) {
    Log(INFO) << "TCPServer accept fd(" << connSocket->get() << ") ip("
              << connSocket->getAddress()->getIP() << ")";

    // new channel
    SharedPtr<Event::Channel> channel(new Event::Channel(connSocket));
    // NOTE setting up channel in connection
    channel->setAfterSetLoopCallback([onMessageCallback = onMessageCallback](
        SharedPtr<Event::Channel> channel) {

        // new connection
        auto connection = MakeShared<TCPConnection>(channel);
        connection->setMessageCallback(onMessageCallback);
        // store this connection in server
        localThreadConnections.insert(connection);

        connection->setShutdownCallback([](SharedPtr<TCPConnection> conn) {
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
