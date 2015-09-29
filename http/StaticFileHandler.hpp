#ifndef HTTP_STATIC_FILE_HANDLER_H
#define HTTP_STATIC_FILE_HANDLER_H

#include "HttpServ.hpp"
#include "Request.hpp"
#include "Response.hpp"


namespace Http {
    class StaticFileHandler final {
    public:
        StaticFileHandler(const std::string&, const std::string&);
        ~StaticFileHandler();

        Response view(const Request&);
        std::string renderDirPage(const std::string&, const std::string&) const;

    private:
        const std::string baseDir;
        const std::string baseUrl;
    };
}

#endif /* HTTP_STATIC_FILE_HANDLER_H */
