#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "HttpServ.hpp"
#include "Request.hpp"
#include "Response.hpp"


namespace Http {
    class RequestHandler {
    public:
        virtual ~RequestHandler() = 0;
        virtual Response get();
        virtual Response post();
        virtual Response put();
        virtual Response head();

    protected:
        unsigned connFd;
        const Request req;

        Response redirect(const std::string&) const;
        Response content(const std::string&) const;
        Response view(const std::string&) const;
        Response sendFile(const std::string&) const;
        Response response(const unsigned&, const std::string&) const;

        friend class Handler;
    };
}


#endif /* HTTP_REQUEST_HANDLER_HPP */
