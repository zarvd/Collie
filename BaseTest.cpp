#include "Httpd.hpp"
#include "base/TcpServer.hpp"
#include "base/Channel.hpp"
#include "base/EventLoop.hpp"
#include "base/Socket.hpp"

using namespace MiniHttp::Base;


int main(int argc, char *argv[]) {
    LoggingHandler.setFlushFrequency(1);
    LoggingHandler.setLogLevel(TRACE);
    LoggingHandler.setLogFile("httpd.log");
    LoggingHandler.setOutput(Logger::LogHandler::Output::FILE, false);
    LoggingHandler.init();

    unsigned port = 8080;
    if(argc == 2) {
        port = atoi(argv[1]);
    }

    TcpServer tcp(port);
    ConnectCallback acceptCB = [&tcp](const int connFd) {
            std::shared_ptr<Channel> connChannel(new Channel(tcp.getEventLoop(), connFd));

            ConnectCallback readCB = [connChannel](const int fd) {
                    const std::string msg = Socket::recv(fd);
                    connChannel->enableWrite();
                    connChannel->setWriteCallback([connChannel](const int fd) {
                            Socket::send(fd, "HTTP/1.1 200 OK\n"
                                         "\n"
                                         "<h1> Hello,wold! </h1>");
                            connChannel->getEventLoop()->removeChannel(connChannel);
                        });
                    Log(ERROR) << connChannel->getEvents();
                    connChannel->getEventLoop()->updateChannel(connChannel);
            };

            connChannel->enableRead();
            connChannel->setReadCallback(readCB);

            Log(ERROR) << connChannel->getEvents();
            tcp.getEventLoop()->updateChannel(connChannel);
    };
    tcp.setConnectCallback(acceptCB);
    tcp.start();

    return 0;
}
