#include "Utils.hpp"


namespace Collie { namespace Utils {

std::string
getAbsolutePath(const std::string& baseUrl,
                const std::string& absoluteUrl,
                const std::string& baseDir) {
    std::string relativeUrl, absoluteDir;
    auto baseUrlVec = split(baseUrl, '/');
    auto absoluteUrlVec = split(absoluteUrl, '/');
    if(baseUrlVec.size() > absoluteUrl.size()) {
        throw std::logic_error("Unknown url");
    }
    for(unsigned idx = 0; idx < baseUrlVec.size(); ++ idx) {
        if(baseUrlVec[idx] != absoluteUrlVec[idx]) {
            throw std::logic_error("Unknown url");
        }
    }
    absoluteUrlVec.erase(absoluteUrlVec.begin(), absoluteUrlVec.begin() + baseUrlVec.size());

    relativeUrl = join(absoluteUrlVec, '/');
    if(relativeUrl.empty()) {
        return baseDir;
    } else if(baseDir.back() == '/') {
        absoluteDir = baseDir + relativeUrl;
    } else {
        absoluteDir = baseDir + '/' + relativeUrl;
    }
    return absoluteDir;
}

}}
