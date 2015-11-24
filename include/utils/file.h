#ifndef COLLIE_UTILS_FILE_H_
#define COLLIE_UTILS_FILE_H_

#include <string>
#include <sys/stat.h>
#include <fcntl.h>

namespace collie {
namespace utils {

class File {
 public:
  using Stat = struct stat64;
  enum class Mode { Read = O_RDONLY, Write = O_WRONLY, ReadAndWrite = O_RDWR };
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

  File(const std::string &pathname, const Mode mode, int flags = 0);
  File(const File &) = delete;
  File &operator=(const File &) = delete;
  ~File();

  bool IsRead() const noexcept {
    return mode_ == Mode::Read || mode_ == Mode::ReadAndWrite;
  }
  bool IsWrite() const noexcept {
    return mode_ == Mode::Write || mode_ == Mode::ReadAndWrite;
  }
  bool IsAbleTo(const int flags) const noexcept {
    return (flags == 0 && flags_ == 0) ? true : (flags & flags_);
  }
  int fd() const noexcept { return fd_; }
  bool is_close() const noexcept { return is_close_; }
  bool is_existed() const noexcept { return is_existed_; }
  bool IsFile() const noexcept { return S_ISREG(stat_.st_mode); }
  bool IsDir() const noexcept { return S_ISDIR(stat_.st_mode); }
  size_t GetSize() const noexcept { return stat_.st_size; }
  void Close() noexcept;

  const std::string kPathName;

 private:
  int fd_;
  bool is_existed_;
  bool is_close_;
  int flags_;
  Mode mode_;

  Stat stat_;
};
}
}

#endif /* COLLIE_UTILS_FILE_H_ */
