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
    LoggingHandler.init();

    unsigned port = 8080;
    if(argc == 2) {
        port = atoi(argv[1]);
    }

    TcpServer tcp(port);
    tcp.setConnectCallback([&tcp](const int connFd) {
            std::shared_ptr<Channel> connChannel(new Channel(tcp.getEventLoop(), connFd));
            // TODO setup the connection callback of this channel
            connChannel->enableRead();
            connChannel->setReadCallback([connChannel](const int fd) {
                    const std::string msg = Socket::recv(fd);
                    connChannel->enableWrite();
                    connChannel->setWriteCallback([](const int fd) {
                            Socket::send(fd, "HTTP/1.1 200 OK\n"
                                         "\n"
                                         "<h1> Hello,wold! </h1>");
                        });
                    Log(ERROR) << connChannel->getEvents();
                });
            Log(ERROR) << connChannel->getEvents();
            tcp.getEventLoop()->updateChannel(connChannel);
        });
    tcp.start();

    return 0;
}
