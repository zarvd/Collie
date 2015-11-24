#include "../../include/poll/poller.h"
#include <unistd.h>

namespace collie {
namespace poll {

Poller::~Poller() {}

void Poller::CloseImpl() noexcept {
  // TODO error handling
  ::close(fd_);
}
}
}
