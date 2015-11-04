#include "../../include/Global.hpp"
#include "../../include/tcp/TcpClient.hpp"
#include "../../include/tcp/Connector.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/event/EventLoop.hpp"

namespace Collie {
namespace Tcp {

TcpClient::TcpClient() : eventLoop(new Event::EventLoop) {

    Log(TRACE) << "TcpClient is constructing";
}

TcpClient::~TcpClient() { Log(TRACE) << "TcpClient is destructing"; }

void
TcpClient::connect(const std::string & host, const unsigned & port) {
    if(!connectCallback) {
        Log(ERROR) << "TcpClient connectCallback is not callable";
        THROW_INVALID_ARGUMENT;
    }
    remoteAddr = SocketAddress::getSocketAddress(host, port);
    connector.reset(new Connector(remoteAddr, eventLoop));
    connector->setConnectCallback(connectCallback);
    connector->connect();
}
}
}
