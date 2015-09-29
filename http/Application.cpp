#include "Application.hpp"


namespace Http {
    Application::Application() :
        tcpSocket(nullptr),
        tcpHandler(nullptr) {

        Log(logLevel::Debug) << "HTTP handler created";
        setDefaultTCPHandler();
    }

    void Application::setDefaultTCPHandler() {
        // init default tcp handler
        tcpHandler = [this](const int& connFd) -> void {
            // FIXME parse header and set route
            // BUG readline and detective if it is a new request
            // TODO maximum header size
            char header[8192];
            recv(connFd, header, 8192, 0);
            Request req = Request::parse(header);

            StaticFileHandler staticHandler("./", "/");
            Response res = staticHandler.view(req);

            char page[res.get().length() + 1];
            std::strcpy(page, res.get().c_str());
            send(connFd, page, sizeof(page), 0);
        };
    }

    /**
     * Initialize a HTTP handler before it runs
     */
    Status Application::init(const unsigned int& port) {
        Log(logLevel::Debug) << "HTTP handler trying to init";
        try {
            tcpSocket = std::unique_ptr<Socket::TcpSocket>(new Socket::TcpSocket);
            tcpSocket->init(port, IP::IPv4);
            tcpSocket->setConnectHandler(tcpHandler);
            Log(logLevel::Debug) << "HTTP handler inited";
            return Status::Success;
        } catch(const std::exception& err) {
            Log(logLevel::Error) << "HTTP handler fail to init";
            Log(logLevel::Error) << err.what();
            return Status::Fail;
        }
    }

    /**
     *
     */
    void Application::run() {
        try {
            Log(logLevel::Info) << "HTTP Server running on port " << tcpSocket->getPort();
            tcpSocket->run();
        } catch(const std::exception& err) {
            Log(logLevel::Error) << "HTTP handler fail to run";
            Log(logLevel::Error) << err.what();
        }
    }

    std::string Application::generateHeader() const {
        return "";
    }
}
