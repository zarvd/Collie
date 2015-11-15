#ifndef COLLIE_UTILS_FILE_H
#define COLLIE_UTILS_FILE_H

#include <string>
#include <sys/stat.h>
#include <fcntl.h>

namespace Collie {
namespace Utils {

class File {
public:
    using Stat = struct stat64;
    enum Flags {
        Read = O_RDONLY,
        Write = O_WRONLY,
        ReadAndWrite = O_RDWR,
        Append = O_APPEND,
        Creat = O_CREAT,
        Excl = O_EXCL,
        Trunc = O_TRUNC,
        Noctty = O_NOCTTY,
        NonBlock = O_NONBLOCK,
        Dsync = O_DSYNC,
        Rsync = O_RSYNC,
        Sync = O_SYNC
    };

    File(const std::string & pathName, const int flags);
    File(const File &) = delete;
    File & operator=(const File &) = delete;
    ~File();

    bool isExisted() const { return existed; }
    bool isFile() const { return S_ISREG(stat.st_mode); }
    bool isDir() const { return S_ISDIR(stat.st_mode); }
    int getFd() const { return fd; }
    size_t getSize() const { return stat.st_size; }
    void close();

private:
    int fd;
    bool existed;
    bool isClose;

    Stat stat;
};
}
}

#endif /* COLLIE_UTILS_FILE_H */
