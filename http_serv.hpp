#ifndef HTTP_SERV_H
#define HTTP_SERV_H

#include "httpd.hpp"
#include "tcp_socket.hpp"
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <sstream>


namespace Http {

    enum class Method {POST, GET, HEAD, PUT, NONE};
    enum class HeaderType {RES, REQ, ENTITY, GENERAL};

    inline Method getMethod(const std::string& str) {
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

    inline std::string getMethod(const Method& method) {
        if(method == Method::GET) {
            return "GET";
        } else if(method == Method::POST) {
            return "POST";
        } else if(method == Method::PUT) {
            return "PUT";
        } else if(method == Method::HEAD) {
            return "HEAD";
        } else {
            return "";
        }
    }

    struct Header {
        std::string field;
        HeaderType type;
        Header() = delete;
        ~Header() = default;
        Header(const std::string&, const HeaderType&);
        bool operator<(const Header&) const;
        bool operator==(const Header&) const;
    };

    extern const std::map<std::string, Header> HTTPHeader;

    // Request body
    struct Request {
        std::map<std::string, std::string> header;
        Method method;
        std::string url;
        std::string httpVersion;

        Request() = default;
        // explicit Request(const std::string&, const Method&);
    };

    extern const std::map<std::string, std::string> MimeType;

    inline std::string getMimeTypeByExt(const std::string& ext) {
        return MimeType.at(ext);
    }

    // get current HTTP Date
    inline std::string getCurrentDate() {
        std::time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char now[80];
        strftime(now, 80, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&nowTime));
        return now;
    }

    std::string getStateByCode(const unsigned short&);

    class HttpHandler;

    // Request Handler
    typedef Status (*RequestHandler)(HttpHandler&, Request&);

    class HttpHandler final {
    private:
        Socket::TcpSocket tcpSocket;

    public:
        std::map<std::string, RequestHandler> router;
        Socket::TcpHandler tcpHandler;

        HttpHandler();
        Status init(const unsigned&);
        Status run();
        std::string generateHeader() const;
    };

    Request parseHeader(const std::string&);
}

#endif /* HTTP_SERV_H */
