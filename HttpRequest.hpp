#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "HttpServ.hpp"


namespace Http {
    // Request body
    struct HttpRequest {
        std::map<std::string, std::string> header;
        Method method;
        std::string url;
        std::string httpVersion;

        HttpRequest() = default;
        static HttpRequest parse(const std::string&);
    };
}

#endif /* HTTPREQUEST_H */
