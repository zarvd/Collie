#include "Header.hpp"


namespace Http {
    namespace Header {
        const std::map<std::string, HeaderType> HttpHeader = {

            // General Header
            {"Cache-Control", HeaderType::GENERAL},
            {"Connection", HeaderType::GENERAL},
            {"Date", HeaderType::GENERAL},
            {"Pragma", HeaderType::GENERAL},
            {"Trailer", HeaderType::GENERAL},
            {"Transfer-Encoding", HeaderType::GENERAL},
            {"Upgrade", HeaderType::GENERAL},
            {"Via", HeaderType::GENERAL},
            {"Warning", HeaderType::GENERAL},

            // Request Header
            {"Accept", HeaderType::REQ},
            {"Accept-Charset", HeaderType::REQ},
            {"Accept-Encoding", HeaderType::REQ},
            {"Accept-Language", HeaderType::REQ},
            {"Authorization", HeaderType::REQ},
            {"Expect", HeaderType::REQ},
            {"From", HeaderType::REQ},
            {"Host", HeaderType::REQ},
            {"If-Match", HeaderType::REQ},
            {"If-Modified-Since", HeaderType::REQ},
            {"If-None-Match", HeaderType::REQ},
            {"If-Range", HeaderType::REQ},
            {"If-Unmodified-Since", HeaderType::REQ},
            {"Max-Forwards", HeaderType::REQ},
            {"Proxy-Authorization", HeaderType::REQ},
            {"Range", HeaderType::REQ},
            {"Referer", HeaderType::REQ},
            {"TE", HeaderType::REQ},
            {"User-Agent", HeaderType::REQ},

            // Response Header
            {"Accept-Ranges", HeaderType::RES},
            {"Age", HeaderType::RES},
            {"ETag", HeaderType::RES},
            {"Location", HeaderType::RES},
            {"Proxy-Authenticate", HeaderType::RES},
            {"Retry-After", HeaderType::RES},
            {"Server", HeaderType::RES},
            {"Vary", HeaderType::RES},
            {"WWW-Authenticate", HeaderType::RES},

            // Entity Header
            {"Allow", HeaderType::ENTITY},
            {"Content-Encoding", HeaderType::ENTITY},
            {"Content-Language", HeaderType::ENTITY},
            {"Content-Length", HeaderType::ENTITY},
            {"Content-Location", HeaderType::ENTITY},
            {"Content-Range", HeaderType::ENTITY},
            {"Content-Type", HeaderType::ENTITY},
            {"Expires", HeaderType::ENTITY},
            {"Last-Modified", HeaderType::ENTITY}
        };
    }
}
