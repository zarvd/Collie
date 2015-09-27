#include "StaticFileHandler.hpp"


namespace Http {
    StaticFileHandler::StaticFileHandler(const std::string& dir,
                                         const std::string& url,
                                         const unsigned& fd) :
        connFd(fd),
        baseDir(Utility::getRealPath(dir)),
        baseUrl(Utility::formatUrl(url)) {}


    StaticFileHandler::~StaticFileHandler() {}

    void StaticFileHandler::view(const Request& req) {
        const std::string path = Utility::getAbsolutePath(baseUrl, req.url, baseDir);
        if(Utility::isDir(path)) {

        }
    }

    std::string StaticFileHandler::renderDirPage(const std::string& dir) const {
        std::set<std::string> fileList = Utility::getFileList(dir);
        return "";
    }
}
