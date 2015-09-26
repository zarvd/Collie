#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "HttpServ.hpp"


namespace Http {
    // Request body
    struct Request {
        std::map<std::string, std::string> header;
        Method method;
        std::string url;
        std::string httpVersion;

        Request();
        static Request parse(const std::string&);
    };
}

#endif /* HTTP_REQUEST_H */
