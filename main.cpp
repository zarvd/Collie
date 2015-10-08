#include "http/Application.hpp"


int main(int argc, char *argv[]) {
    try {
        initHttpd();
        unsigned port = 8080;
        if(argc == 2) {
            port = atoi(argv[1]);
        }
        Http::Application http;
        http.init(port);
        http.run();
    } catch(const std::exception& err) {
        Log(ERROR) << err.what();
    }
    return 0;
}
