#include "../../include/udp/UDPServer.hpp"
#include "../../include/udp/UDPSocket.hpp"
#include "../../include/SocketAddress.hpp"
#include "../../include/Global.hpp"

namespace Collie {
namespace UDP {

UDPServer::UDPServer() {}
UDPServer::~UDPServer() {}

void
UDPServer::bind(const std::string & host, const unsigned port) {
    Log(DEBUG) << "UDP is binding";

    localAddr = SocketAddress::getSocketAddress(host, port);
    socket.reset(new UDPSocket(localAddr));
    socket->listen();
}

void
UDPServer::start() {
    Log(INFO) << "UDP Server is starting";
    REQUIRE(connectCallback);
    while(true) {
        std::string content;
        auto remoteAddr = std::make_shared<SocketAddress>();
        socket->recvFrom(content, remoteAddr);
        connectCallback(content, remoteAddr);
    }
}

void
UDPServer::send(const std::string & content,
                std::shared_ptr<SocketAddress> remoteAddr) {
    REQUIRE(socket);
    socket->sendTo(content, remoteAddr);
}
}
}
