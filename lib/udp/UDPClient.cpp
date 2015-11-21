#include "../../include/InetAddress.hpp"
#include "../../include/udp/UDPSocket.hpp"
#include "../../include/udp/UDPClient.hpp"
#include "../../include/Global.hpp"

namespace Collie {
namespace UDP {

UDPClient::UDPClient() {}
UDPClient::~UDPClient() {}

void
UDPClient::connect(const std::string & host, const unsigned port,
                   const std::string & greeting) {
    REQUIRE(connectCallback);
    socket.reset(new UDPSocket(nullptr));
    remoteAddr = InetAddress::getInetAddress(host, port);
    socket->connect(remoteAddr->getIPVersion());
    // socket->sendTo(greeting, remoteAddr);
    std::string content;
    // socket->recvFrom(content, remoteAddr);
    while(connectCallback(content, remoteAddr)) {
        // loop until return false
    }
}

void
UDPClient::send(const std::string & content,
                SharedPtr<InetAddress> remoteAddr) {
    REQUIRE(socket);
    // socket->sendTo(content, remoteAddr);
}
}
}
