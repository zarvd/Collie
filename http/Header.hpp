#ifndef HTTP_HEADER_H
#define HTTP_HEADER_H

#include "HttpServ.hpp"


namespace Http {
    namespace Header {
        extern const std::map<std::string, HeaderType> HEADER;

        inline std::string generateHeader(const std::map<std::string, std::string>& header) {
            std::string str;
            for(const auto& it : header) {
                str += it.first + ":" + it.second + "\n";
            }
            str += "\n";
            return str;
        }
    }
}

#endif /* HTTP_HEADER_H */
