#include "../../include/Global.hpp"
#include "../../include/tcp/TcpClient.hpp"
#include "../../include/tcp/Connector.hpp"
#include "../../include/SocketAddress.hpp"


namespace Collie { namespace Tcp {

TcpClient::TcpClient() {}

TcpClient::~TcpClient() {}

void
TcpClient::connect(const std::string & host, const unsigned & port) {
    if( ! connectCallback) {
        Log(ERROR) << "TcpClient connectCallback is not callable";
        THROW_INVALID_ARGUMENT;
    }
    remoteAddr = SocketAddress::getSocketAddress(host, port);
    connector.reset(new Connector(remoteAddr));
    connector->setConnectCallback(connectCallback);
}

}}
