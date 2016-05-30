#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/fcntl.h>
#include "../inc/socket.h"
#include "../inc/logger.h"

namespace collie {

Socket::Socket() noexcept : fd(-1) {}

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
