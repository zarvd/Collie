#include "../include/udp/UDPSocket.hpp"
#include "../include/udp/UDPClient.hpp"
#include "../include/SocketAddress.hpp"
#include "../include/Global.hpp"

using Collie::UDP::UDPClient;
using namespace Collie;

int
main(int argc, char * argv[]) {
    auto & logger = Logger::LogHandler::getHandler();
    logger.setLogLevel(TRACE);
    logger.init();

    unsigned port = 8080;
    if(argc == 2) port = std::stoul(argv[1]);

    UDPClient client;
    client.setConnectCallback([&client](const std::string & content,
                                        std::shared_ptr<SocketAddress> addr) {
        const auto res = "From UDP Echo Client: " + content;
        client.send(content, addr);
        return false;
    });
    client.connect("127.0.0.1", port, "Hello world");
    return 0;
}
