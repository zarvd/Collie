#ifndef HTTP_STATIC_FILE_HANDLER_H
#define HTTP_STATIC_FILE_HANDLER_H


#include "HttpServ.hpp"
#include "Request.hpp"
#include "Response.hpp"

namespace Http {
    class StaticFileHandler final {
    public:
        explicit StaticFileHandler(const std::string&, const Request&);
        ~StaticFileHandler();

        Response & get() const;

    private:
        const std::string dir;
        const Request req;
    };
}

#endif /* HTTP_STATIC_FILE_HANDLER_H */
