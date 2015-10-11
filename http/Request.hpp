#ifndef MINIHTTP_HTTP_REQUEST_HPP
#define MINIHTTP_HTTP_REQUEST_HPP

#include "Common.hpp"


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
#endif /* MINIHTTP_HTTP_REQUEST_HPP */
