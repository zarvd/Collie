#include "httpd.hpp"


void initLogging() {
    LoggingHandler.setLogFile("httpd.log");
    LoggingHandler.init();
}

void initHttpd() {
    initLogging();
}
