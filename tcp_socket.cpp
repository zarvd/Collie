#include "tcp_socket.hpp"

namespace Socket {
    TcpSocket::TcpSocket() {
        ipVersion = IP::None;
        socketFd = 0;
    }

    inline IP TcpSocket::getIPVersion() const {
        return ipVersion;
    }

    Status TcpSocket::init(const int& port, const IP& ipVersion) {
        Status st = (ipVersion == IP::IPv4) ? initIPv4(port) : initIPv6(port);
        if(st == Status::Success) {
            this->ipVersion = ipVersion;
        }

        return st;
    }

    Status TcpSocket::initIPv4(const int& port) {
        struct sockaddr_in servAddr;

        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(port);
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        int listenFd;
        listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(listenFd) {
            // TODO exception handler
            return Status::Fail;
        }

        if(bind(listenFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
            return Status::Fail;
        }

        if(listen(listenFd, SOMAXCONN) < 0) {
            return Status::Fail;
        }

        if(socketFd && close(socketFd)) {
            // close a existed socket
            return Status::Fail;
        }

        socketFd = listenFd;

        return Status::Success;
    }

    Status TcpSocket::initIPv6(const int&) {
        // TODO
        return Status::Fail;
    }

    Status TcpSocket::setHandler(std::shared_ptr<TcpHandler> handler) {
        if(handler == nullptr || handler->handler) {
            return Status::Fail;
        }

        this->handler = handler;
        return Status::Success;
    }

    Status TcpSocket::run() {
        if( ! socketFd || ipVersion == IP::None || handler == nullptr) {
            return Status::Fail;
        }

        while(true) {
            struct sockaddr * clientAddr;
            int connFd;
            pid_t pid;
            socklen_t clientAddrLen;

            clientAddr = (struct sockaddr *) malloc (sizeof(struct sockaddr));
            clientAddrLen = sizeof(clientAddr);

            connFd = accept(socketFd, clientAddr, &clientAddrLen);

            if(connFd < 0) {
                return Status::Fail;
            }

            if((pid = fork()) < 0) {
                return Status::Fail;
            } else if(pid == 0) {
                // child process
                close(socketFd);
                handler->handler(connFd);  // use handler
                close(connFd);
                exit(0);  // exit child process
            }

            // TODO: avoid zombie child process
            // parent process
            close(connFd);
        }
    }
}
