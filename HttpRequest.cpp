#include "HttpRequest.hpp"


namespace Http {
    HttpRequest HttpRequest::parse(const std::string& str) {
        std::stringstream in(str);
        HttpRequest req;
        std::string line;
        std::getline(in, line);  // get url, method and httpVersion
        auto head = StringHelper::split(line, ' ');
        if(head.size() != 3) {
            std::string err("Unknow header field: " + line);
            throw std::length_error(err);
        }
        req.method = getMethod(head[0]);
        req.url = head[1];  // TODO split GET data
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
