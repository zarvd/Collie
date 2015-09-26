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
            char header[3000];
            recv(connFd, header, 3000, 0);
            Request req = Request::parse(header);

            RequestHandler handler;
            auto it = router.find({req.url, req.method});
            if(it != router.end()) {
                handler = it->second;
                handler(req);
            } else {
                // no router rule
                // response 404
                char response[] =
                "HTTP/1.1 404 Not Found\n"
                "\n"
                "<h1>404 Not Found</h1>";
                send(connFd, response, sizeof(response), 0);
            }
            // if(req.url == "/" && req.method == Method::GET) {
            //     char greeting[] =
            //         "HTTP/1.1 200 OK\n"
            //         "Content-Type: text/html\n"
            //         "Server: miniHttpd\n"
            //         "\n"
            //         "<h1>Hello, world</h1>";
            //     send(connFd, greeting, sizeof(greeting), 0);
            // } else {
            // }
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
