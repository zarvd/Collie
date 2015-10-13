#include "base/Global.hpp"
#include "base/TcpServer.hpp"
#include "base/Channel.hpp"
#include "base/EventLoop.hpp"
#include "base/Socket.hpp"

using namespace Collie::Base;


void testSocket(const unsigned port) {
    Socket socket(port);
    socket.listen();
    Log(INFO) << "Socket " << socket.getFd() << " is listening " << port;
    std::shared_ptr<SocketAddress> addr(new SocketAddress);
    int connFd = socket.accept(addr);
    Log(INFO) << "Socket accept " << connFd;
    const std::string msg = Socket::recv(connFd);
    Log(INFO) << "Connection " << connFd << " recevied " << msg;
    Socket::send(connFd, "HTTP/1.1 200 OK\n"
                 "\n"
                 "<h1>hello world!</h1>");
    socket.close();
}

void testEPoller(const unsigned) {
}

void testEventLoop(const unsigned port) {
    std::shared_ptr<EventLoop> eventLoop(new EventLoop);
    Socket socket(port);
    socket.listen();
    std::shared_ptr<Channel> acceptChannel(new Channel(eventLoop, socket.getFd()));
    eventLoop->updateChannel(acceptChannel);
    acceptChannel->enableRead();
    acceptChannel->setReadCallback([eventLoop](const unsigned listenFd) {

            std::shared_ptr<SocketAddress> addr(new SocketAddress);
            int connFd = Socket::accept(listenFd, addr);
            Log(INFO) << "new connection " << connFd;

            // create new channel
            std::shared_ptr<Channel> connChannel(new Channel(eventLoop, connFd));
            // insert in to eventLoop
            eventLoop->updateChannel(connChannel);

            // read event
            connChannel->enableRead();
            connChannel->setReadCallback([connChannel](const unsigned fd) {
                    Log(INFO) << "Connection " << fd << " is reading";
                    const std::string msg = Socket::recv(fd);
                    std::cout << msg << std::endl;
                    connChannel->disableRead();

                    // write event
                    connChannel->enableWrite();
                    connChannel->setWriteCallback([connChannel](const unsigned fd) {
                            Log(INFO) << "Connection " << fd << " is writing";
                            Socket::send(fd, "HTTP/1.1 200 OK\n"
                                         "Content-Type: text/html\n"
                                         "Content-Length: 21\n"
                                         "\n"
                                         "<h1>hello world!</h1>");
                            connChannel->disableWrite();
                            connChannel->enableRead();
                        });

                });

            // close event
            connChannel->setCloseCallback([connChannel](const unsigned fd) {
                    Log(INFO) << "Connection " << fd << " is closed";
                    connChannel->remove();
                });

            // error event
            connChannel->setErrorCallback([connChannel](const unsigned fd) {
                    Log(INFO) << "Connection " << fd << " meets error";
                    connChannel->remove();
                });
        });
    eventLoop->loop();
}

void testTcpServer(const unsigned port) {
    TcpServer tcp(port);

    ConnectCallback acceptCB = [&tcp](const int connFd) {
        std::shared_ptr<Channel> connChannel(new Channel(tcp.getEventLoop(), connFd));

        ConnectCallback readCB = [connChannel](const int fd) {
            const std::string msg = Socket::recv(fd);
            std::cout << msg << std::endl;
            connChannel->enableWrite();
            connChannel->setWriteCallback([connChannel](const int fd) {
                    Socket::send(fd, "HTTP/1.1 404 Not Found\n"
                                 "Content-Type: text/html\n"
                                 "Content-Length: 22\n"
                                 "\n"
                                 "<h1> Hello,wold! </h1>");
                    connChannel->remove();
                });
            connChannel->update();
        };

        connChannel->enableRead();
        connChannel->setReadCallback(readCB);

        tcp.getEventLoop()->updateChannel(connChannel);  // FIXME
    };
    tcp.setConnectCallback(acceptCB);
    tcp.start();
}

int main(int argc, char *argv[]) {
    LoggingHandler.setFlushFrequency(1);
    LoggingHandler.setLogLevel(TRACE);
    // LoggingHandler.setLogFile("httpd.log");
    LoggingHandler.setOutput(Logger::LogHandler::Output::FILE, false);
    LoggingHandler.init();

    unsigned port = 8080;
    if(argc == 2) {
        port = atoi(argv[1]);
    }
    // testSocket(port);
    // testEventLoop(port);
    testTcpServer(port);
    return 0;
}
