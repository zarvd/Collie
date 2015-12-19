#ifndef COLLIE_UTILS_FILE_H_
#define COLLIE_UTILS_FILE_H_

#include <string>
#include <sys/stat.h>
#include <fcntl.h>

namespace collie {
namespace utils {

// Opens file and manipulates I/O operations
class File {
 public:
  using Stat = struct stat64;
  enum class Mode {
    READ = O_RDONLY,
    WRITE = O_WRONLY,
    READ_AND_WRITE = O_RDWR
  };
  enum Flags {
    APPEND = O_APPEND,
    CREAT = O_CREAT,
    EXCL = O_EXCL,
    TRUNC = O_TRUNC,
    NO_CTTY = O_NOCTTY,
    NON_BLOCK = O_NONBLOCK,
    D_SYNC = O_DSYNC,
    R_SYNC = O_RSYNC,
    SYNC = O_SYNC
  };

  File(const std::string &pathname, const Mode mode, int flags = 0) noexcept;
  File(const File &) = delete;
  File &operator=(const File &) = delete;
  ~File() noexcept;

  bool IsRead() const noexcept {
    return mode_ == Mode::READ || mode_ == Mode::READ_AND_WRITE;
  }
  bool IsWrite() const noexcept {
    return mode_ == Mode::WRITE || mode_ == Mode::READ_AND_WRITE;
  }
  bool IsAbleTo(const int flags) const noexcept {
    return (flags == 0 && flags_ == 0) ? true : (flags & flags_);
  }
  int fd() const noexcept { return fd_; }
  bool is_close() const noexcept { return is_close_; }
  // Returns `true` when the file or directory exists
  bool is_existed() const noexcept { return is_existed_; }
  // Returns `true` when it's a file
  bool IsFile() const noexcept { return S_ISREG(stat_.st_mode); }
  // Returns `true` when it's a directory
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
