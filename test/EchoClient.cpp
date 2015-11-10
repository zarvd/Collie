#include <iostream>
#include <string>
#include "../include/tcp/TcpClient.hpp"
#include "../include/tcp/TcpSocket.hpp"
#include "../include/Global.hpp"

using Collie::Tcp::TcpClient;
using Collie::Tcp::TcpSocket;
using namespace Collie;

int
main(int argc, char * argv[]) {

    auto & logger = Logger::LogHandler::getHandler();
    logger.setLogLevel(TRACE);
    logger.init();

    unsigned port = 8080;
    if(argc == 2) port = std::stoul(argv[1]);

    TcpClient client;
    client.setConnectCallback([](std::shared_ptr<TcpSocket> socket) {
        socket->send("Hello, here is client");
        const auto content = socket->recv();
        std::cout << content << std::endl;
    });
    client.connect("127.0.0.1", port);
    return 0;
}
