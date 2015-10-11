#include "Httpd.hpp"


void initLogging() {
    LoggingHandler.setFlushFrequency(1);
    LoggingHandler.setLogLevel(INFO);
    LoggingHandler.setLogFile("httpd.log");
    LoggingHandler.init();
}

void initHttpd() {
    initLogging();
}
