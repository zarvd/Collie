#include "../include/tcp/TCPServer.hpp"
#include "../include/tcp/TCPConnection.hpp"
#include "../include/InetAddress.hpp"
#include "../include/Global.hpp"
#include "../include/utils/File.hpp"

using Collie::TCP::TCPServer;
using Collie::TCP::TCPConnection;
using Collie::Utils::File;
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
        conn->recvAll();
        File file("/home/gallon/collie/send/test.xls", File::Mode::Read);
        if(!file.isExisted() || !file.isFile()) {
            Log(WARN) << file.getName() << " not found";
            return;
        }
        Log(INFO) << "File size = " << file.getSize();
        Log(INFO) << "Sending file...";
        conn->sendFile(file);
    });
    server.start();

    return 0;
}
