#include "Response.hpp"


namespace Http {
    Response::Response(const std::string& content) :
        status(200),
        content(content) {}

    Response::Response(const unsigned& status) :
        status(status),
        content(genContent(status)) {}

    Response::Response(const unsigned& status, const std::string& content) :
        status(status),
        content(content) {}

    Response::~Response() {}

    std::string Response::get() const {
        return content;
    }

    unsigned Response::getStatus() const {
        return status;
    }

    std::string Response::genContent(const unsigned& status) {
        std::string statusDetail = getStatusByCode(status);
        const std::string title = std::to_string(status) + " " + statusDetail;
        return
            "HTTP/1.1 " + title + "\n"
            "\n"
            "<html>"
            "<head><title>" + title + "</title></head>"
            "<body><h1>" + title + "</h1></body>"
            "</html>";
    }
}
