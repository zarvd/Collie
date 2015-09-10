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

    struct Header {
        std::string field;
        std::string content;
        HeaderType type;
        Header() = default;
        ~Header() = default;
        Header(const std::string&, const HeaderType&);
        bool operator<(const Header&) const;
        bool operator==(const Header&) const;
        bool operator<(const std::string&) const;
        bool operator==(const std::string&) const;
    };

    extern const std::map<std::string, Header> HTTPHeader;

    // Request body
    struct Request {
        std::map<std::string, std::string> header;
        Method method;
        std::string url;
        std::string httpVersion;

        Request() = delete;
        explicit Request(const std::string&, const Method&);
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

    // Request Handler
    typedef Status (*RequestHandler)(Request&);

    class HttpHandler final {
    private:
        Socket::TcpSocket tcpSocket;

    public:
        std::map<std::string, RequestHandler> router;
        Socket::TcpHandler tcpHandler;

        HttpHandler();
        Status init(const unsigned&);
        Status run();
        std::set<Header> parseHeader(const std::string&) const;
        std::string generateHeader() const;
    };

}

#endif /* HTTP_SERV_H */
