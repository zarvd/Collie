#include "../../include/Global.hpp"
#include "../../include/tcp/Connector.hpp"
#include "../../include/tcp/TcpSocket.hpp"
#include "../../include/SocketAddress.hpp"


namespace Collie { namespace Tcp {

Connector::Connector(std::shared_ptr<SocketAddress> addr) :
    remoteAddr(addr) {
    Log(TRACE) << "Connector is constructing";
}

Connector::~Connector() {}

void
Connector::socket() {
    tcpSocket.reset(new TcpSocket);
}

void
Connector::connect() {
    if( ! tcpSocket) socket();
    tcpSocket->connect(remoteAddr);
    connectCallback(tcpSocket->getFd());
}

}}
