#ifndef HTTPD_H
#define HTTPD_H

#include "logging/Logger.hpp"


using Logger::LoggingHandler;
using logLevel = Logger::Level;

typedef const std::string& constStrRef;
typedef const std::string constStr;

enum class IP {None, IPv4, IPv6};
enum class Status {Fail, Success};

void initHttpd();


#endif /* HTTPD_H */
