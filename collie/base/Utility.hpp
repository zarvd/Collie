#ifndef UTILITY_H
#define UTILITY_H

#include <set>
#include <string>
#include <exception>
#include <dirent.h>
#include <sys/stat.h>
#include "StringHelper.hpp"


namespace MiniHttp { namespace Base { namespace Utility {

std::string getAbsolutePath(const std::string&, const std::string&, const std::string&);

inline std::string getRealPath(const std::string& path) {
    char absoluteDir[200];
    realpath(path.c_str(), absoluteDir);
    return absoluteDir;
}

inline std::string formatUrl(const std::string& url) {
    if(url.back() == '/' && url.length() != 1) {
        return url.substr(0, url.length() - 1);
    } else {
        return url;
    }
}

inline bool isDir(const std::string& path) {
    struct stat buf;
    if(stat(path.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode)) {
        return true;
    } else {
        return false;
    }
}

inline std::set<std::string> getFileList(const std::string& path) {
    std::set<std::string> files;
    DIR * dir;
    if((dir = opendir(path.c_str())) != NULL) {
        struct dirent * file;
        while((file = readdir (dir)) != NULL) {
            files.insert(file->d_name);
        }
        closedir(dir);
    } else {
        throw std::runtime_error("Open dirctory error");
    }
    return files;
}}}}

#endif /* UTILITY_H */
