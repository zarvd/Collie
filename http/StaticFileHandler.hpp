#ifndef HTTP_STATIC_FILE_HANDLER_H
#define HTTP_STATIC_FILE_HANDLER_H

#include "../httpd.hpp"
#include "Request.hpp"
#include "Utility.hpp"
#include <cstring>


namespace Http {
    class StaticFileHandler final {
    public:
        StaticFileHandler(const std::string&,
                          const std::string&,
                          const unsigned&);
        ~StaticFileHandler();

        void view(const Request&);
        std::string renderDirPage(const std::string&, const std::string&) const;

    private:
        const unsigned connFd;
        const std::string baseDir;
        const std::string baseUrl;
    };
}

#endif /* HTTP_STATIC_FILE_HANDLER_H */
