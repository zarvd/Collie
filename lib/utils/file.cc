#include "../../include/utils/file.h"
#include "../../include/logging.h"
#include <unistd.h>

namespace collie {
namespace utils {

File::File(const std::string& pathname, const Mode mode, int flags)
    : kPathName(pathname),
      fd_(-1),
      is_existed_(true),
      is_close_(false),
      flags_(flags),
      mode_(mode) {
  if (::stat64(pathname.c_str(), &stat_) == -1) {
    if (errno == ENOENT && flags & Flags::Creat) {
      // file is not existed and flags contains CREAT
      // then create a new file
      const int creatFlags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
      int ret = ::creat64(pathname.c_str(), creatFlags);
      if (ret == -1) {
        LOG(WARNING) << "Path" << pathname << "can not be created"
                     << ":" << GetSystemError();
        is_existed_ = false;
      } else {
        LOG(INFO) << "Create file" << pathname;
        ::stat64(pathname.c_str(), &stat_);  // refresh stat;
        ::close(ret);  // creat() set ONLY WRITE, we need to reopen it
      }
    }
  }
  if (is_existed_ && S_ISREG(stat_.st_mode)) {
    // is file
    fd_ = ::open64(pathname.c_str(), static_cast<int>(mode), flags);
    if (fd_ != -1) {
      LOG(INFO) << "Open file" << pathname;
    } else {
      LOG(WARNING) << "Can not open file" << pathname << ":"
                   << GetSystemError();
    }
  }
}

File::~File() {
  if (!is_close_ && IsFile()) Close();
}

void File::Close() noexcept {
  if (!is_close_ && IsFile()) ::close(fd_);
}
}
}
