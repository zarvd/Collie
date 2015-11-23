#include <iostream>
#include <string>
#include "../include/tcp/TCPServer.hpp"
#include "../include/tcp/TCPConnection.hpp"
#include "../include/InetAddress.hpp"
#include "../include/Global.hpp"

using Collie::TCP::TCPServer;
using Collie::TCP::TCPConnection;
using namespace Collie;

int
main(int argc, char * argv[]) {

    auto & logger = Logger::LogHandler::getHandler();
    logger.setLogLevel(TRACE);
    logger.init();

    unsigned port = 8080;
    if(argc == 2) port = std::stoul(argv[1]);

    TCPServer server;
    server.bind("0.0.0.0", port);
    server.setOnMessageCallback([](std::shared_ptr<TCPConnection> conn) {
        REQUIRE(conn);
        const auto content = conn->recvAll();
        auto addr = conn->getRemoteAddress();
        REQUIRE(addr);
        Log(INFO) << addr->getIP() << " (" << addr->getPort()
                  << "): " << content;
        conn->send("Hi, here is server");
    });
    server.start();
    return 0;
}
