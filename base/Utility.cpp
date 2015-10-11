#include "Utility.hpp"


namespace Utility {

std::string getAbsolutePath(const std::string& baseUrl, const std::string& absoluteUrl, const std::string& baseDir) {
    std::string relativeUrl, absoluteDir;
    auto baseUrlVec = StringHelper::split(baseUrl, '/');
    auto absoluteUrlVec = StringHelper::split(absoluteUrl, '/');
    if(baseUrlVec.size() > absoluteUrl.size()) {
        throw std::logic_error("Unknown url");
    }
    for(unsigned idx = 0; idx < baseUrlVec.size(); ++ idx) {
        if(baseUrlVec[idx] != absoluteUrlVec[idx]) {
            throw std::logic_error("Unknown url");
        }
    }
    absoluteUrlVec.erase(absoluteUrlVec.begin(), absoluteUrlVec.begin() + baseUrlVec.size());

    relativeUrl = StringHelper::join(absoluteUrlVec, '/');
    if(relativeUrl.empty()) {
        return baseDir;
    } else if(baseDir.back() == '/') {
        absoluteDir = baseDir + relativeUrl;
    } else {
        absoluteDir = baseDir + '/' + relativeUrl;
    }
    return absoluteDir;
}
}
