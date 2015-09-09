#ifndef HTTPD_H
#define HTTPD_H

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "./logging/logger.hpp"

using Logger::LoggingHandler;
using Logger::logInfo;
using Logger::logDebug;
using Logger::logWarning;
using Logger::logError;

enum IP {
    // FIXME: use enum class
    None,
    IPv4,
    IPv6
};

enum Status {
    // FIXME: use enum class
    Fail,
    Success
};

void initHttpServer();

#endif /* HTTPD_H */
