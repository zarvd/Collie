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
#include <exception>
#include "./logging/logger.hpp"


using Logger::LoggingHandler;
using Logger::logInfo;
using Logger::logDebug;
using Logger::logWarning;
using Logger::logError;

enum class IP {None, IPv4, IPv6};

enum class Status {Fail, Success};

void initHttpServer();

inline std::string trim(const std::string& str) {
    short begin, end;
    begin = -1;
    end = str.length() - 1;
    for(std::size_t idx = 0; idx < str.length(); ++ idx) {
        if(isspace(str[idx])) {
            begin = idx;
        } else {
            break;
        }
    }
    for(std::size_t idx = str.length(); idx >= 1; --idx) {
        if(isspace(str[idx - 1])) {
            end = idx;
        } else {
            break;
        }
    }
    return str.substr(begin + 1, end - begin + 1);
}

#endif /* HTTPD_H */
