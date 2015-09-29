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
            const std::string body = renderDirPage(req.url, path);
            const std::string header =
                "HTTP/1.1 200 OK\n"
                "\n";
            const std::string pageStr = (header + body);
            char page[pageStr.length() + 1];
            std::strcpy(page, pageStr.c_str());
            send(connFd, page, sizeof(page), 0);
        } else {
            Log(logLevel::Warn) << path << " is not directory";
            char page[] =
                "HTTP/1.1 404 Not Found\n"
                "\n"
                "<html>"
                "<head><title>404 Not Found</title></head>"
                "<body><h1>404 Not Found</h1></body>"
                "</html>";
            send(connFd, page, sizeof(page), 0);
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
