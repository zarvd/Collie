#include "../../include/Global.hpp"
#include "../../include/tcp/TcpClient.hpp"
#include "../../include/tcp/Connector.hpp"
#include "../../include/SocketAddress.hpp"

namespace Collie {
namespace Tcp {

TcpClient::TcpClient() { Log(TRACE) << "TcpClient is constructing"; }

TcpClient::~TcpClient() {
    Log(TRACE) << "TcpClient is destructing";
    if(connector) connector->disconnect();
}

void
TcpClient::connect(const std::string & host, const unsigned port,
                   const size_t threadNum, const size_t connectNum) {
    if(!connectCallback) {
        THROW_("TcpClient connectCallback is not callable");
    }

    remoteAddr = SocketAddress::getSocketAddress(host, port);
    connector.reset(new Connector(remoteAddr));
    connector->setConnectCallback(connectCallback);
    connector->connect(threadNum, connectNum);
}
}
}
