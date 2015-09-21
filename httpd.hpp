#ifndef HTTPD_H
#define HTTPD_H

#include <iostream>
#include <algorithm>
#include <string>
#include <memory>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <exception>
#include <vector>
#include "./logging/logger.hpp"


using Logger::LoggingHandler;
using logLevel = Logger::Level;

enum class IP {None, IPv4, IPv6};

enum class Status {Fail, Success};

void initHttpd();

namespace StringHelper {
    // remove all spaces form string
    inline void removeSpace(std::string& str) {
        str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
    }

    // remove front and back spaceS from string
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

    // split string into vector of string by a char
    inline std::vector<std::string> split(const std::string& str, const char& sym) {
        unsigned short begin;
        begin = 0;
        std::vector<std::string> vec;
        for(std::size_t idx = 0; idx < str.length(); ++ idx) {
            const char& curChar = str[idx];
            if(curChar == sym) {
                vec.push_back(trim(str.substr(begin, idx - begin)));
                begin = idx;
            }
        }
        vec.push_back(trim(str.substr(begin)));
        return vec;
    }
}

#endif /* HTTPD_H */
