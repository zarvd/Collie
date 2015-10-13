#include "StaticFileHandler.hpp"
#include "../httpd.hpp"
#include "../base/Utility.hpp"



namespace MiniHttp { namespace Http {

StaticFileHandler::StaticFileHandler(const std::string& dir,
                                     const std::string& url) :
    baseDir(Base::Utility::getRealPath(dir)),
    baseUrl(Base::Utility::formatUrl(url)) {}


StaticFileHandler::~StaticFileHandler() {}

Response StaticFileHandler::view(const Request& req) {
    const std::string path = Base::Utility::getAbsolutePath(baseUrl, req.url, baseDir);
    if(Base::Utility::isDir(path)) {
        const std::string body = renderDirPage(req.url, path);
        return Response(body);
    } else {
        Log(WARN) << path << " is not directory";
        return Response(404);
    }
}

std::string StaticFileHandler::renderDirPage(const std::string& url, const std::string& dir) const {
    std::set<std::string> fileList = Base::Utility::getFileList(dir);
    std::string title = "Index of " + url;
    std::string page = "<html><head><title>" + title + "</title></head><body><h1>" + title + "</h1><table>";
    for(const auto& file : fileList) {
        page += "<tr><td>" + file + "</td></tr>";
    }
    page += "</table></body></html>";
    return page;
}

}}
