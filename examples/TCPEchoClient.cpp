#include <iostream>
#include <string>
#include "../include/tcp/TCPClient.hpp"
#include "../include/tcp/TCPSocket.hpp"
#include "../include/Global.hpp"

using Collie::TCP::TCPClient;
using Collie::TCP::TCPSocket;
using namespace Collie;

int
main(int argc, char * argv[]) {

    auto & logger = Logger::LogHandler::getHandler();
    logger.setLogLevel(TRACE);
    logger.init();

    unsigned port = 8080;
    if(argc == 2) port = std::stoul(argv[1]);

    TCPClient client;
    client.setConnectCallback([](std::shared_ptr<TCPSocket> socket) {
        socket->send("Hello, here is client");
        std::string content;
        socket->recv(content);
        Log(INFO) << "Received: " << content;
        socket->send("Goodbye");
        socket->recv(content);
        Log(INFO) << "Received: " << content;

    });
    client.connect("127.0.0.1", port, 4, 100);
    return 0;
}
