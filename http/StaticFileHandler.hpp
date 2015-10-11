#ifndef MINIHTTP_STATICFILEHANDLER_HPP
#define MINIHTTP_STATICFILEHANDLER_HPP

#include "Common.hpp"
#include "Request.hpp"
#include "Response.hpp"


namespace MiniHttp { namespace Http {

class StaticFileHandler {
public:
    StaticFileHandler(const std::string &, const std::string &);
    StaticFileHandler(const StaticFileHandler &) = delete;
    StaticFileHandler operator=(const StaticFileHandler &) = delete;
    ~StaticFileHandler();

    Response view(const Request &);
    std::string renderDirPage(const std::string &, const std::string &) const;

private:
    const std::string baseDir;
    const std::string baseUrl;
};

}}

#endif /* MINIHTTP_STATICFILEHANDLER_HPP */
