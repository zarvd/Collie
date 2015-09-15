#include "tcp_socket.hpp"

namespace Socket {
    TcpSocket::TcpSocket() {
        ipVersion = IP::None;
        socketFd = 0;
    }

    TcpSocket::~TcpSocket() {
        if(socketFd > 2) {
            close(socketFd);
        }
    }

    inline IP TcpSocket::getIPVersion() const {
        return ipVersion;
    }

    Status TcpSocket::init(const unsigned int& port, const IP& ipVersion) {
        Status st = (ipVersion == IP::IPv4) ? initIPv4(port) : initIPv6(port);
        if(st == Status::Success) {
            this->ipVersion = ipVersion;
        }

        return st;
    }

    Status TcpSocket::initIPv4(const unsigned int& port) {
        struct sockaddr_in servAddr;

        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(port);
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        int listenFd;
        // FIXME close socket when recv terminal singal
        listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(listenFd < 0) {
            // TODO exception handler
            throw std::runtime_error("Socket failed in creating");
            return Status::Fail;
        }

        if(bind(listenFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
            throw std::runtime_error("Socket failed in binding");
            return Status::Fail;
        }

        if(listen(listenFd, SOMAXCONN) < 0) {
            throw std::runtime_error("Socket failed in listening");
            return Status::Fail;
        }

        if(socketFd && close(socketFd)) {
            // close a existed socket
            throw std::logic_error("Socket already created");
            return Status::Fail;
        }

        socketFd = listenFd;

        return Status::Success;
    }

    Status TcpSocket::initIPv6(const unsigned int&) {
        // TODO
        return Status::Fail;
    }

    Status TcpSocket::setHandler(std::shared_ptr<TcpHandler> handler) {
        // if(handler == nullptr || handler->handler) {
        //     return Status::Fail;
        // }

        this->handler = handler;
        return Status::Success;
    }

    Status TcpSocket::run() {
        if(socketFd < 0 || ipVersion == IP::None || handler == nullptr) {
            return Status::Fail;
        }

        while(true) {
            // TODO multithreading, multiprocessing, I/O comletion
            struct sockaddr_in clientAddr;
            int connFd;
            socklen_t clientAddrLen;

            clientAddrLen = sizeof(clientAddr);

            logInfo("socket connecting");
            connFd = accept(socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);

            if(connFd < 0) {
                throw std::runtime_error("socket connection error");
            }

            // TODO IPv6
            char clientIP[80];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));

            logInfo("socket connected from " + std::string(clientIP));
            handler->handler(connFd);
            close(connFd);
            logInfo("socket connection closed");
        }
    }
}
