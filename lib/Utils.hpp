#ifndef COLLIE_UTILS_H
#define COLLIE_UTILS_H

#include <set>
#include <string>
#include <algorithm>
#include <sstream>
#include <exception>
#include <dirent.h>
#include <sys/stat.h>


namespace Collie { namespace Utils {

std::string
getAbsolutePath(const std::string & baseUrl,
                const std::string & absoluteUrl,
                const std::string & baseDir);

// remove all spaces form string
inline void
removeSpace(std::string & str) {
    str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}

// remove front and back spaceS from string
inline std::string
trim(const std::string & str) {
    short begin, end;
    begin = -1;
    end = str.length() - 1;
    for(std::size_t idx = 0; idx < str.length(); ++ idx) {
        if(isspace(str[idx])) {
            begin = idx;
        } else {
            break;
        }
    }
    for(std::size_t idx = str.length(); idx >= 1; --idx) {
        if(isspace(str[idx - 1])) {
            end = idx;
        } else {
            break;
        }
    }
    return str.substr(begin + 1, end - begin + 1);
}

inline std::vector<std::string> &
split(const std::string & s, const char & delim, std::vector<std::string> & elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if(item != "")
            elems.push_back(item);
    }
    return elems;
}

inline std::vector<std::string>
split(const std::string & s, const char & delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

inline std::string
join(const std::vector<std::string> & v, const char & c) {
    std::stringstream ss;
    for(size_t i = 0; i < v.size(); ++i)
    {
        if(i != 0) ss << c;
        ss << v[i];
    }
    std::string s = ss.str();
    return s;
}

inline std::string
getRealPath(const std::string & path) {
    char absoluteDir[200];
    realpath(path.c_str(), absoluteDir);
    return absoluteDir;
}

inline std::string
formatUrl(const std::string & url) {
    if(url.back() == '/'  && url.length() != 1) {
        return url.substr(0, url.length() - 1);
    } else {
        return url;
    }
}

inline bool
isDir(const std::string & path) {
    struct stat buf;
    if(stat(path.c_str(),  &buf) == 0  && S_ISDIR(buf.st_mode)) {
        return true;
    } else {
        return false;
    }
}

inline std::set<std::string>
getFileList(const std::string & path) {
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
}

}}


#endif /* COLLIE_UTILS_H */
