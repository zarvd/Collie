#include "Httpd.hpp"
#include "base/TcpServer.hpp"

using namespace MiniHttp::Base;


int main(int argc, char *argv[]) {
    initHttpd();

    unsigned port = 8080;
    if(argc == 2) {
        port = atoi(argv[1]);
    }

    TcpServer tcp(port);
    tcp.listen().start();

    return 0;
}
