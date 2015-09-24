#include "HttpHandler.hpp"


int main(int argc, char *argv[]) {
    try {
        LoggingHandler.setFlushFrequency(1);
        LoggingHandler.setLogLevel(logLevel::Debug);
        LoggingHandler.setLogFile("httpd.log");
        LoggingHandler.init();
        unsigned port = 8080;
        if(argc == 2) {
            port = atoi(argv[1]);
        }
        Http::HttpHandler http;
        http.init(port);
        http.run();
    } catch(const std::exception& err) {
        Log(logLevel::Error) << err.what();
    }
    return 0;
}
