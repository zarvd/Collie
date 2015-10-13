#ifndef MINIHTTP_HTTP_COMMON_HPP
#define MINIHTTP_HTTP_COMMON_HPP

#include <map>
#include <ctime>
#include <chrono>


namespace MiniHttp { namespace Http {

extern const std::map<std::string, std::string> MimeType;
enum class Method {POST, GET, HEAD, PUT, NONE};
enum class HeaderType {RES, REQ, ENTITY, GENERAL};

inline Method getMethod(const std::string & str) {
    if(str == "GET") {
        return Method::GET;
    } else if(str == "POST") {
        return Method::POST;
    } else if(str == "PUT") {
        return Method::PUT;
    } else if(str == "HEAD") {
        return Method::HEAD;
    } else {
        return Method::NONE;
    }
}

inline std::string getMethod(const Method & method) {
    switch(method) {
    case Method::GET: return "GET";
    case Method::POST: return "POST";
    case Method::PUT: return "PUT";
    case Method::HEAD: return "HEAD";
    case Method::NONE: return "NONE";
    }
}

// get current Date in HTTP format
inline std::string getCurrentDate() {
    std::time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char now[80];
    strftime(now, 80, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&nowTime));
    return now;
}

std::string getStatusByCode(const unsigned short &);
}}

#endif /* MINIHTTP_HTTP_COMMON_HPP */
