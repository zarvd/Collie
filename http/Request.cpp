#include "Request.hpp"


namespace Http {
    Request::Request() :
        method(Method::NONE),
        url(),
        httpVersion() {}

    Request Request::parse(const std::string& str) {
        std::stringstream in(str);
        Request req;
        std::string line;

        std::getline(in, line);  // get url, method and httpVersion

        auto head = StringHelper::split(line, ' ');
        if(head.size() != 3) {
            Log(logLevel::Warn) << "Unknow header field: " << line;
            return req;
        }
        req.method = getMethod(head[0]);
        req.url = head[1];
        req.httpVersion = head[2];

        while(std::getline(in, line)) {
            StringHelper::removeSpace(line);
            const std::size_t foundPos = line.find(':');
            if(foundPos != std::string::npos) {
                std::string field = line.substr(0, foundPos);
                std::string content = line.substr(foundPos + 1);
                req.header[field] = content;
            }
        }
        return req;
    }
}
