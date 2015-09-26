#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

#include "HttpServ.hpp"


namespace Http {
    struct Header {
        std::string field;
        HeaderType type;

        Header() = delete;
        ~Header() = default;
        Header(const std::string&, const HeaderType&);

        bool operator<(const Header&) const;
        bool operator==(const Header&) const;
    };

    extern const std::map<std::string, Header> HTTPHeader;
}

#endif /* HTTP_HEADER_H */
