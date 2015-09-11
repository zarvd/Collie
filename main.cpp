#include "http_serv.hpp"


int main(int argc, char *argv[]) {
    Http::HttpHandler http;
    http.init(8080);
    http.run();
    return 0;
}
