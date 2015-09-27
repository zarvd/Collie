#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "HttpServ.hpp"


namespace Http {
    class Response {
    public:
        virtual ~Response() = 0;

    protected:
        std::pair<unsigned, std::string> status;
        std::map<std::string, std::string> header;

        void setStatus(const unsigned&);
        void setHeader(const std::string&, const std::string&);
    };

    class View : public Response {
    public:
        explicit View(const std::string&);
        ~View();

        Response & status(const unsigned&);
        Response & header(const std::string&, const std::string&);
        Response & cookie(const std::string&, const std::string&);
    };

    class Content : public Response {
    };

    class Redirect : public Response {
    };
}

#endif /* HTTP_RESPONSE_H */
