#include "StaticFileHandler.hpp"


namespace Http {
    StaticFileHandler::StaticFileHandler(const std::string& dir,
                                         const std::string& url) :
        baseDir(Utility::getRealPath(dir)),
        baseUrl(Utility::formatUrl(url)) {}


    StaticFileHandler::~StaticFileHandler() {}

    Response StaticFileHandler::view(const Request& req) {
        const std::string path = Utility::getAbsolutePath(baseUrl, req.url, baseDir);
        if(Utility::isDir(path)) {
            const std::string body = renderDirPage(req.url, path);
            return Response(body);
        } else {
            Log(logLevel::Warn) << path << " is not directory";
            return Response(404);
        }
    }

    std::string StaticFileHandler::renderDirPage(const std::string& url, const std::string& dir) const {
        std::set<std::string> fileList = Utility::getFileList(dir);
        std::string title = "Index of " + url;
        std::string page = "<html><head><title>" + title + "</title></head><body><h1>" + title + "</h1><table>";
        for(const auto& file : fileList) {
            page += "<tr><td>" + file + "</td></tr>";
        }
        page += "</table></body></html>";
        return page;
    }
}
