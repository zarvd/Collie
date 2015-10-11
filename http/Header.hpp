#ifndef MINIHTTP_HTTP_HEADER_HPP
#define MINIHTTP_HTTP_HEADER_HPP

#include "Common.hpp"


namespace MiniHttp { namespace Http {namespace Header {

extern const std::map<std::string, HeaderType> HEADER;

inline std::string generateHeader(const std::map<std::string, std::string>& header) {
    std::string str;
    for(const auto& it : header) {
        str += it.first + ":" + it.second + "\n";
    }
    str += "\n";
    return str;
}

}}}


#endif /* MINIHTTP_HTTP_HEADER_HPP */
