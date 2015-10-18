#include "../../../include/Global.hpp"
#include "../../../include/network/tcp/Connector.hpp"
#include "../../../include/network/tcp/TcpSocket.hpp"
#include "../../../include/network/SocketAddress.hpp"


namespace Collie { namespace Network { namespace Tcp {

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

}}}
