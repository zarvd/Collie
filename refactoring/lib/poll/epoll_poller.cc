#include "../../inc/poll/epoll_poller.h"
#include "../../inc/logger.h"
#include <sys/epoll.h>
#include <unistd.h>

namespace collie {

EPollPoller::EPollPoller() : Poller() {}
EPollPoller::~EPollPoller() noexcept {}

void EPollPoller::Init() throw(PollException) {
  if (poll_fd_ != -1) {
    LOG(WARN) << "EPollPoller is already inited";
    return;
  }
  poll_fd_ = ::epoll_create1(0);
  if (poll_fd_ == -1) {
    throw PollException("EPoll cannot init");
  }
}

void EPollPoller::Destroy() throw(PollException) {
  if (poll_fd_ == -1) {
    LOG(WARN) << "EPollPoller is already destoried";
    return;
  }
  if (::close(poll_fd_) == -1) {
    throw PollException("EPoll cannot close");
  } else {
    poll_fd_ = -1;
  }
}

void EPollPoller::Insert(const Descriptor& fd,
                         Events events) throw(PollException) {
  ASSERT(poll_fd_ != -1)

  ::epoll_event e;
  e.data.fd = fd.GetDescriptor();
  e.events = events;
  if (::epoll_ctl(poll_fd_, EPOLL_CTL_ADD, e.data.fd, &e) == -1) {
    throw PollException("EPoll cannot insert");
  }
}

void EPollPoller::Update(const Descriptor& fd,
                         Events events) throw(PollException) {
  ASSERT(poll_fd_ != -1)

  ::epoll_event e;
  e.data.fd = fd.GetDescriptor();
  e.events = events;
  if (::epoll_ctl(poll_fd_, EPOLL_CTL_MOD, e.data.fd, &e) == -1) {
    throw PollException("EPoll cannot update");
  }
}
void EPollPoller::Delete(const Descriptor& fd) throw(PollException) {
  ASSERT(poll_fd_ != -1)

  if (::epoll_ctl(poll_fd_, EPOLL_CTL_DEL, fd.GetDescriptor(), nullptr) == -1) {
    throw PollException("EPoll cannot delete");
  }
}

void EPollPoller::Poll(PollHandler& handler, int timeout) throw(PollException) {
  ASSERT(poll_fd_ != -1)

  int size = max_event_num_;
  ::epoll_event revents[size];
  int num = ::epoll_wait(poll_fd_, revents, max_event_num_, timeout);
  if (num == -1) {
    throw PollException("EPoll cannot poll");
  }

  for (int i = 0; i < num; ++i) {
    ::epoll_event& e = revents[i];
    handler(e.data.fd, e.events);
  }
}
}
