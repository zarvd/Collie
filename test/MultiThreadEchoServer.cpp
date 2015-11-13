#include <iostream>
#include <string>
#include "../include/tcp/TCPServer.hpp"
#include "../include/tcp/TCPConnection.hpp"
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
    server.setThreadNum(2);
    server.setOnMessageCallback([](std::shared_ptr<TCPConnection> conn) {
        const auto content = conn->recvAll();

        std::cout << content << std::endl;
        conn->send("Hi, here is server");
    });
    server.start();
    return 0;
}
