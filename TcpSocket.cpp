#include "TcpSocket.hpp"


namespace Socket {
    TcpSocket::TcpSocket() :
        ipVersion(IP::None),
        listenFd(0),
        port(0),
        connectHandler(nullptr) {}

    TcpSocket::~TcpSocket() {
        if(listenFd > 2) {
            close(listenFd);
        }
    }

    IP TcpSocket::getIPVersion() const {
        return ipVersion;
    }

    unsigned TcpSocket::getPort() const {
        return port;
    }

    Status TcpSocket::init(const unsigned& port, const IP& ipVersion) {
        Status status = (ipVersion == IP::IPv4) ? initIPv4(port) : initIPv6(port);
        if(status == Status::Success) {
            this->port = port;
            this->ipVersion = ipVersion;
        } else {
            Log(logLevel::Warn) << "TcpSocket init error";
        }

        return status;
    }

    Status TcpSocket::initIPv4(const unsigned& port) {
        struct sockaddr_in servAddr;

        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(port);
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        int err;
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
        if(listenFd < 0 || ipVersion == IP::None || connectHandler == nullptr) {
            return Status::Fail;
        }


        while(true) {
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen;
            clientAddrLen = sizeof(clientAddr);

            int connFd;
            Log(logLevel::Debug) << "TcpSocket running";
            connFd = accept(listenFd, (struct sockaddr *)&clientAddr, &clientAddrLen);

            if(connFd < 0) {
                throw std::runtime_error("TcpSocket connection error");
            }

            // TODO IPv6
            char clientIP[80];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));

            Log(logLevel::Info) << "TcpSocket connected from " + std::string(clientIP);
            connectHandler(connFd);
            close(connFd);
            Log(logLevel::Debug) << "TcpSocket connection closed";
        }
    }
}
