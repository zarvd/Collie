#include "Header.hpp"


namespace Http {
    Header::Header(const std::string& field, const HeaderType& type) : field(field), type(type) {}
    bool Header::operator<(const Header& that) const {
        return field < that.field;
    }

    bool Header::operator==(const Header& that) const {
        return field == that.field;
    }

    const std::map<std::string, Header> HTTPHeader = {

        // General Header
        {"Cache-Control", Header("Cache-Control", HeaderType::GENERAL)},
        {"Connection", Header("Connection", HeaderType::GENERAL)},
        {"Date", Header("Date", HeaderType::GENERAL)},
        {"Pragma", Header("Pragma", HeaderType::GENERAL)},
        {"Trailer", Header("Trailer", HeaderType::GENERAL)},
        {"Transfer-Encoding", Header("Transfer-Encoding", HeaderType::GENERAL)},
        {"Upgrade", Header("Upgrade", HeaderType::GENERAL)},
        {"Via", Header("Via", HeaderType::GENERAL)},
        {"Warning", Header("Warning", HeaderType::GENERAL)},

        // Request Header
        {"Accept", Header("Accept", HeaderType::REQ)},
        {"Accept-Charset", Header("Accept-Charset", HeaderType::REQ)},
        {"Accept-Encoding", Header("Accept-Encoding", HeaderType::REQ)},
        {"Accept-Language", Header("Accept-Language", HeaderType::REQ)},
        {"Authorization", Header("Authorization", HeaderType::REQ)},
        {"Expect", Header("Expect", HeaderType::REQ)},
        {"From", Header("From", HeaderType::REQ)},
        {"Host", Header("Host", HeaderType::REQ)},
        {"If-Match", Header("If-Match", HeaderType::REQ)},
        {"If-Modified-Since", Header("If-Modified-Since", HeaderType::REQ)},
        {"If-None-Match", Header("If-None-Match", HeaderType::REQ)},
        {"If-Range", Header("If-Range", HeaderType::REQ)},
        {"If-Unmodified-Since", Header("If-Unmodified-Since", HeaderType::REQ)},
        {"Max-Forwards", Header("Max-Forwards", HeaderType::REQ)},
        {"Proxy-Authorization", Header("Proxy-Authorization", HeaderType::REQ)},
        {"Range", Header("Range", HeaderType::REQ)},
        {"Referer", Header("Referer", HeaderType::REQ)},
        {"TE", Header("TE", HeaderType::REQ)},
        {"User-Agent", Header("User-Agent", HeaderType::REQ)},

        // Response Header
        {"Accept-Ranges", Header("Accept-Ranges", HeaderType::RES)},
        {"Age", Header("Age", HeaderType::RES)},
        {"ETag", Header("ETag", HeaderType::RES)},
        {"Location", Header("Location", HeaderType::RES)},
        {"Proxy-Authenticate", Header("Proxy-Authenticate", HeaderType::RES)},
        {"Retry-After", Header("Retry-After", HeaderType::RES)},
        {"Server", Header("Server", HeaderType::RES)},
        {"Vary", Header("Vary", HeaderType::RES)},
        {"WWW-Authenticate", Header("WWW-Authenticate", HeaderType::RES)},

        // Entity Header
        {"Allow", Header("Allow", HeaderType::ENTITY)},
        {"Content-Encoding", Header("Content-Encoding", HeaderType::ENTITY)},
        {"Content-Language", Header("Content-Language", HeaderType::ENTITY)},
        {"Content-Length", Header("Content-Length", HeaderType::ENTITY)},
        {"Content-Location", Header("Content-Location", HeaderType::ENTITY)},
        {"Content-Range", Header("Content-Range", HeaderType::ENTITY)},
        {"Content-Type", Header("Content-Type", HeaderType::ENTITY)},
        {"Expires", Header("Expires", HeaderType::ENTITY)},
        {"Last-Modified", Header("Last-Modified", HeaderType::ENTITY)}
    };
}
