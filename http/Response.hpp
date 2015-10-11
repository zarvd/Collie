#ifndef MINIHTTP_HTTP_RESPONSE_HPP
#define MINIHTTP_HTTP_RESPONSE_HPP

#include "Common.hpp"


namespace MiniHttp { namespace Http {

class Response {
public:
    Response() = delete;
    explicit Response(const unsigned&);  // status code
    explicit Response(const std::string&);  // content with default status 200
    Response(const unsigned&, const std::string&);  // status code and content
    virtual ~Response();

    virtual std::string get() const;
    unsigned getStatus() const;
    static std::string genContent(const unsigned&);

protected:
    unsigned status;
    std::string content;
    std::map<std::string, std::string> headerField;  // TODO
};

}}

#endif /* MINIHTTP_HTTP_RESPONSE_HPP */
