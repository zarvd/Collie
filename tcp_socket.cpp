#include "tcp_socket.hpp"

namespace Socket {
    TcpSocket::TcpSocket() :
        connectHandler(nullptr) {
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

    Status TcpSocket::init(const unsigned& port, const IP& ipVersion) {
        Status st = (ipVersion == IP::IPv4) ? initIPv4(port) : initIPv6(port);
        if(st == Status::Success) {
            this->ipVersion = ipVersion;
        }

        return st;
    }

    Status TcpSocket::initIPv4(const unsigned& port) {
        struct sockaddr_in servAddr;

        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(port);
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        int listenFd, err;
        listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(listenFd < 0) {
            throw std::runtime_error("Socket failed in creating");
        }

        err = bind(listenFd, (struct sockaddr *)&servAddr, sizeof(servAddr));
        if(err < 0) {
            throw std::runtime_error("Socket failed in binding");
        }

        err = listen(listenFd, SOMAXCONN);
        if(err) {
            throw std::runtime_error("Socket failed in listening");
        }

        if(socketFd && close(socketFd)) {
            // close a existed socket
            throw std::logic_error("Socket already created");
        }

        socketFd = listenFd;

        return Status::Success;
    }

    Status TcpSocket::initIPv6(const unsigned int&) {
        // TODO
        return Status::Fail;
    }

    Status TcpSocket::setConnectHandler(Handler handler) {
        if( ! handler) {
            return Status::Fail;
        }

        this->connectHandler = handler;
        return Status::Success;
    }

    Status TcpSocket::run() {
        if(socketFd < 0 || ipVersion == IP::None || connectHandler == nullptr) {
            return Status::Fail;
        }

        while(true) {
            // TODO multithreading, multiprocessing, I/O comletion
            struct sockaddr_in clientAddr;
            int connFd;
            socklen_t clientAddrLen;

            clientAddrLen = sizeof(clientAddr);

            Log(logLevel::Info) << "socket connecting";
            connFd = accept(socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);

            if(connFd < 0) {
                throw std::runtime_error("socket connection error");
            }

            // TODO IPv6
            char clientIP[80];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));

            Log(logLevel::Info) << "socket connected from " + std::string(clientIP);
            connectHandler(connFd);
            close(connFd);
            Log(logLevel::Info) << "socket connection closed";
        }
    }
}
