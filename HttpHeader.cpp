#include "HttpHeader.hpp"

namespace Http {
    HttpHeader::HttpHeader(const std::string& field, const HeaderType& type) : field(field), type(type) {}
    bool HttpHeader::operator<(const HttpHeader& that) const {
        return field < that.field;
    }

    bool HttpHeader::operator==(const HttpHeader& that) const {
        return field == that.field;
    }

    const std::map<std::string, HttpHeader> HTTPHeader = {

        // General Header
        {"Cache-Control", HttpHeader("Cache-Control", HeaderType::GENERAL)},
        {"Connection", HttpHeader("Connection", HeaderType::GENERAL)},
        {"Date", HttpHeader("Date", HeaderType::GENERAL)},
        {"Pragma", HttpHeader("Pragma", HeaderType::GENERAL)},
        {"Trailer", HttpHeader("Trailer", HeaderType::GENERAL)},
        {"Transfer-Encoding", HttpHeader("Transfer-Encoding", HeaderType::GENERAL)},
        {"Upgrade", HttpHeader("Upgrade", HeaderType::GENERAL)},
        {"Via", HttpHeader("Via", HeaderType::GENERAL)},
        {"Warning", HttpHeader("Warning", HeaderType::GENERAL)},

        // Request Header
        {"Accept", HttpHeader("Accept", HeaderType::REQ)},
        {"Accept-Charset", HttpHeader("Accept-Charset", HeaderType::REQ)},
        {"Accept-Encoding", HttpHeader("Accept-Encoding", HeaderType::REQ)},
        {"Accept-Language", HttpHeader("Accept-Language", HeaderType::REQ)},
        {"Authorization", HttpHeader("Authorization", HeaderType::REQ)},
        {"Expect", HttpHeader("Expect", HeaderType::REQ)},
        {"From", HttpHeader("From", HeaderType::REQ)},
        {"Host", HttpHeader("Host", HeaderType::REQ)},
        {"If-Match", HttpHeader("If-Match", HeaderType::REQ)},
        {"If-Modified-Since", HttpHeader("If-Modified-Since", HeaderType::REQ)},
        {"If-None-Match", HttpHeader("If-None-Match", HeaderType::REQ)},
        {"If-Range", HttpHeader("If-Range", HeaderType::REQ)},
        {"If-Unmodified-Since", HttpHeader("If-Unmodified-Since", HeaderType::REQ)},
        {"Max-Forwards", HttpHeader("Max-Forwards", HeaderType::REQ)},
        {"Proxy-Authorization", HttpHeader("Proxy-Authorization", HeaderType::REQ)},
        {"Range", HttpHeader("Range", HeaderType::REQ)},
        {"Referer", HttpHeader("Referer", HeaderType::REQ)},
        {"TE", HttpHeader("TE", HeaderType::REQ)},
        {"User-Agent", HttpHeader("User-Agent", HeaderType::REQ)},

        // Response Header
        {"Accept-Ranges", HttpHeader("Accept-Ranges", HeaderType::RES)},
        {"Age", HttpHeader("Age", HeaderType::RES)},
        {"ETag", HttpHeader("ETag", HeaderType::RES)},
        {"Location", HttpHeader("Location", HeaderType::RES)},
        {"Proxy-Authenticate", HttpHeader("Proxy-Authenticate", HeaderType::RES)},
        {"Retry-After", HttpHeader("Retry-After", HeaderType::RES)},
        {"Server", HttpHeader("Server", HeaderType::RES)},
        {"Vary", HttpHeader("Vary", HeaderType::RES)},
        {"WWW-Authenticate", HttpHeader("WWW-Authenticate", HeaderType::RES)},

        // Entity Header
        {"Allow", HttpHeader("Allow", HeaderType::ENTITY)},
        {"Content-Encoding", HttpHeader("Content-Encoding", HeaderType::ENTITY)},
        {"Content-Language", HttpHeader("Content-Language", HeaderType::ENTITY)},
        {"Content-Length", HttpHeader("Content-Length", HeaderType::ENTITY)},
        {"Content-Location", HttpHeader("Content-Location", HeaderType::ENTITY)},
        {"Content-Range", HttpHeader("Content-Range", HeaderType::ENTITY)},
        {"Content-Type", HttpHeader("Content-Type", HeaderType::ENTITY)},
        {"Expires", HttpHeader("Expires", HeaderType::ENTITY)},
        {"Last-Modified", HttpHeader("Last-Modified", HeaderType::ENTITY)}
    };
}
