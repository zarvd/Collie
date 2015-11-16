#include "../../include/Global.hpp"
#include "../../include/utils/file.hpp"
#include <unistd.h>

namespace Collie {
namespace Utils {

File::File(const std::string & pathName, const int flags)
    : fd(-1), existed(true), isClose(false) {
    if(::stat64(pathName.c_str(), &stat) == -1) {
        Log(WARN) << "Path " << pathName << " is not existed";
        existed = false;
    } else if(S_ISREG(stat.st_mode)) {
        // is file
        fd = ::open64(pathName.c_str(), flags);
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
