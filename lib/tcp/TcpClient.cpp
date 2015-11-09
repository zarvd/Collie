#include "../../include/Global.hpp"
#include "../../include/tcp/TcpClient.hpp"
#include "../../include/tcp/Connector.hpp"
#include "../../include/SocketAddress.hpp"

namespace Collie {
namespace Tcp {

TcpClient::TcpClient() { Log(TRACE) << "TcpClient is constructing"; }

TcpClient::~TcpClient() { Log(TRACE) << "TcpClient is destructing"; }

void
TcpClient::connect(const std::string & host, const unsigned & port,
                   const size_t threadNum, const size_t connectNum) {
    if(!connectCallback) {
        Log(ERROR) << "TcpClient connectCallback is not callable";
        THROW;
    }

    remoteAddr = SocketAddress::getSocketAddress(host, port);
    connector.reset(new Connector(remoteAddr));
    connector->setConnectCallback(connectCallback);
    connector->connect(threadNum, connectNum);
}
}
}
