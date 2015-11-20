#include "../../include/Global.hpp"
#include "../../include/utils/File.hpp"
#include <unistd.h>

namespace Collie {
namespace Utils {

File::File(const std::string & pathName, const Mode mode, int flags)
    : fd(-1),
      existed(true),
      isClose(false),
      flags(flags),
      mode(mode),
      fileName(pathName) {

    if(::stat64(pathName.c_str(), &stat) == -1) {
        if(errno == ENOENT && flags & Flags::Creat) {
            // file is not existed and flags contains CREAT
            // then create a new file
            const int creatFlags = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
            int ret = ::creat64(pathName.c_str(), creatFlags);
            if(ret == -1) {
                Log(WARN) << "Path " << pathName << " can not be created"
                          << " : " << getError();
                existed = false;
            } else {
                Log(INFO) << "Create file " << pathName;
                ::stat64(pathName.c_str(), &stat); // refresh stat;
                ::close(ret); // creat() set ONLY WRITE, we need to reopen it
            }
        }
    }
    if(existed && S_ISREG(stat.st_mode)) {
        // is file
        fd = ::open64(pathName.c_str(), static_cast<int>(mode), flags);
        if(fd != -1) {
            Log(INFO) << "Open file " << pathName;
        } else {
            Log(WARN) << "Can not open file " << pathName << " : "
                      << getError();
        }
    }
}

File::~File() {
    if(!isClose && isFile()) close();
}

void
File::close() noexcept {
    if(!isClose && isFile()) ::close(fd);
}
}
}
