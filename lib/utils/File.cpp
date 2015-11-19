#include "../../include/Global.hpp"
#include "../../include/utils/File.hpp"
#include <unistd.h>

namespace Collie {
namespace Utils {

File::File(const std::string & pathName, const Mode mode)
    : fd(-1),
      existed(true),
      isClose(false),
      flags(0),
      mode(mode),
      fileName(pathName) {
    if(::stat64(pathName.c_str(), &stat) == -1) {
        if(errno == ENOENT) {
            int ret = ::creat64(pathName.c_str(),
                                S_ISUID | S_ISGID | S_IRWXU); // FIXME
            if(ret == -1) {
                Log(WARN) << "Path " << pathName << " can not be created"
                          << " : " << getError();
                existed = false;
            } else {
                Log(INFO) << "Create file " << pathName;
                ::stat64(pathName.c_str(), &stat);
                ::close(ret);
            }
        }
    }
    if(S_ISREG(stat.st_mode)) {
        // is file
        fd = ::open64(pathName.c_str(), static_cast<int>(mode), flags); // BUG
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
File::close() {
    if(!isClose && isFile()) ::close(fd);
}
}
}
