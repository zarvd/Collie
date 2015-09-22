#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#include "HttpServ.hpp"


namespace Http {
    struct HttpHeader {
        std::string field;
        HeaderType type;
        HttpHeader() = delete;
        ~HttpHeader() = default;
        HttpHeader(const std::string&, const HeaderType&);
        bool operator<(const HttpHeader&) const;
        bool operator==(const HttpHeader&) const;
    };

    extern const std::map<std::string, HttpHeader> HTTPHeader;
}

#endif /* HTTPHEADER_H */
