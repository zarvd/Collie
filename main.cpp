#include "HttpHandler.hpp"


int main(void) {
    Http::HttpHandler http;
    http.init(8080);
    http.run();
    return 0;
}
