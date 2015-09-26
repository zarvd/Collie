#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "HttpServ.hpp"


namespace Http {
    class Response {
    public:
        virtual ~Response() = 0;

        Status setStatus(const unsigned&);
        Status setHeader(const std::string&, const std::string&);
        Status response();

    protected:
        unsigned statusCode;
        std::map<std::string, std::string> header;
    };

    class View : public Response {
    };

    class Content : public Response {
    };

    class Redirect : public Response {
    };
}

#endif /* HTTP_RESPONSE_H */
