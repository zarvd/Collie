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
    enum class Mode {
        Read = O_RDONLY,
        Write = O_WRONLY,
        ReadAndWrite = O_RDWR
    };
    enum Flags {
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

    File(const std::string & pathName, const Mode mode, int flags = 0);
    File(const File &) = delete;
    File & operator=(const File &) = delete;
    ~File();

    bool isRead() const noexcept {
        return mode == Mode::Read || mode == Mode::ReadAndWrite;
    }
    bool isWrite() const noexcept {
        return mode == Mode::Write || mode == Mode::ReadAndWrite;
    }
    bool isAbleTo(const int flags) const noexcept {
        return (flags == 0 && this->flags == 0) ? true : (flags & this->flags);
    }
    bool isExisted() const noexcept { return existed; }
    bool isFile() const noexcept { return S_ISREG(stat.st_mode); }
    bool isDir() const noexcept { return S_ISDIR(stat.st_mode); }
    int getFd() const noexcept { return fd; }
    size_t getSize() const noexcept { return stat.st_size; }
    std::string getName() const noexcept { return fileName; }
    void close() noexcept;

private:
    int fd;
    bool existed;
    bool isClose;
    int flags;
    Mode mode;
    const std::string fileName;

    Stat stat;
};
}
}

#endif /* COLLIE_UTILS_FILE_H */
