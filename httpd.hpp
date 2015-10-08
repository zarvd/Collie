#ifndef HTTPD_H
#define HTTPD_H

#include "logging/Logger.hpp"


using Logger::LoggingHandler;
using Logger::Level::DEBUG;
using Logger::Level::INFO;
using Logger::Level::WARN;
using Logger::Level::ERROR;

typedef const std::string& constStrRef;
typedef const std::string constStr;

enum class IP {NONE, IPv4, IPv6};
enum class Status {FAIL, SUCCESS};

void initHttpd();


#endif /* HTTPD_H */
