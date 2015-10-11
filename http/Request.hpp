#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "HttpServ.hpp"


namespace MiniHttp { namespace Http {

// Request body
class Request {
public:
    Request();
    static Request parse(const std::string&);

    std::map<std::string, std::string> header;
    Method method;
    std::string url;
    std::string httpVersion;
};

}}
#endif /* HTTP_REQUEST_H */
