#include "../../inc/base/Socket.h"
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../inc/base/Logger.h"

namespace collie {
namespace base {

Socket::~Socket() noexcept {
  if (fd != -1) {
    Close();
  }
}

void Socket::SetNonBlocking() noexcept {
  // FIXME
  if (::fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
    LOG(ERROR) << "Cannot set " << fd << " non-blocking";
  }
}

void Socket::Close() noexcept {
  if (fd != -1) {
    if (::shutdown(fd, SHUT_RDWR) < 0) {
      LOG(WARN) << "TCP shutdown: " << ::strerror(errno);
    }
    if (::close(fd) < 0) {
      LOG(WARN) << "TCP close: " << ::strerror(errno);
    }
    fd = -1;
  }
}
}
}
