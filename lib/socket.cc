#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "../inc/socket.h"
#include "../inc/logger.h"

namespace collie {

Socket::Socket() noexcept : fd_(-1) {}

Socket::~Socket() noexcept {
  if (fd_ != -1) {
    Close();
  }
}

void Socket::Close() noexcept {
  if (fd_ != -1) {
    if (::shutdown(fd_, SHUT_RDWR) < 0) {
      LOG(WARN) << "TCP shutdown: " << ::strerror(errno);
    }
    if (::close(fd_) < 0) {
      LOG(WARN) << "TCP close: " << ::strerror(errno);
    }
    fd_ = -1;
  }
}
}
