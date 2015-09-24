#include "httpd.hpp"


void initLogging() {
    LoggingHandler.setFlushFrequency(1);
    LoggingHandler.setLogLevel(logLevel::Info);
    LoggingHandler.setLogFile("httpd.log");
    LoggingHandler.init();
}

void initHttpd() {
    initLogging();
}
