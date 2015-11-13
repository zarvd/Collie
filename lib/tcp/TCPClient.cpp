#include "../../include/Global.hpp"
#include "../../include/tcp/TCPClient.hpp"
#include "../../include/tcp/Connector.hpp"
#include "../../include/SocketAddress.hpp"

namespace Collie {
namespace TCP {

TCPClient::TCPClient() { Log(TRACE) << "TCPClient is constructing"; }

TCPClient::~TCPClient() {
    Log(TRACE) << "TCPClient is destructing";
    if(connector) connector->disconnect();
}

void
TCPClient::connect(const std::string & host, const unsigned port,
                   const size_t threadNum, const size_t connectNum) {
    if(!connectCallback) {
        THROW_("TCPClient connectCallback is not callable");
    }

    remoteAddr = SocketAddress::getSocketAddress(host, port);
    connector.reset(new Connector(remoteAddr));
    connector->setConnectCallback(connectCallback);
    connector->connect(threadNum, connectNum);
}
}
}
