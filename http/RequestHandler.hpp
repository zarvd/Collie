#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "HttpServ.hpp"
#include "Response.hpp"


namespace Http {
    class RequestHandler {
    public:
        virtual ~RequestHandler();
        virtual Response get();
        virtual Response post();
        virtual Response put();
        virtual Response head();

    protected:
        unsigned connFd;

        Response response(const std::string&);
        Response view(const std::string&);
        Response sendError(const unsigned&, const std::string&);

        friend class Handler;
    };
}


#endif /* HTTP_REQUEST_HANDLER_HPP */
