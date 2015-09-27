#ifndef HTTPD_H
#define HTTPD_H

#include <iostream>
#include <algorithm>

#include <string>
#include <vector>

#include <thread>
#include <memory>
#include <exception>
#include <functional>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sstream>
#include "logging/Logger.hpp"


using Logger::LoggingHandler;
using logLevel = Logger::Level;

typedef const std::string& constStrRef;
typedef const std::string constStr;

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

    inline std::vector<std::string>& split(const std::string& s, const char& delim, std::vector<std::string>& elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            if(item != "")
                elems.push_back(item);
        }
        return elems;
    }

    inline std::vector<std::string> split(const std::string& s, const char& delim) {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

    inline std::string join(const std::vector<std::string>& v, const char& c) {
        std::stringstream ss;
        for(size_t i = 0; i < v.size(); ++i)
        {
            if(i != 0) ss << c;
            ss << v[i];
        }
        std::string s = ss.str();
        return s;
    }
}

#endif /* HTTPD_H */
