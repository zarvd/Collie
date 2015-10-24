#include "../../include/Global.hpp"
#include "../../include/tcp/Connector.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/tcp/TcpConnection.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/event/EventLoop.hpp"


namespace Collie { namespace Tcp {

Connector::Connector(std::shared_ptr<SocketAddress> addr,
                     std::shared_ptr<Event::EventLoop> eventLoop) :
    eventLoop(eventLoop),
    remoteAddr(addr) {
    Log(TRACE) << "Connector is constructing";
}

Connector::~Connector() {
    Log(TRACE) << "Connector is destructing";
}

void
Connector::socket() {
    tcpSocket.reset(new TcpSocket);
}

void
Connector::connect() {
    if( ! tcpSocket) socket();
    tcpSocket->connect(remoteAddr);
    std::shared_ptr<TcpConnection> conn(new TcpConnection(eventLoop,
                                                          tcpSocket->getFd(),
                                                          tcpSocket->getLocalAddr(),
                                                          remoteAddr));
    connectCallback(conn);
}

}}
