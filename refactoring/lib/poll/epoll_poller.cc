#include "../../inc/poll/epoll_poller.h"
#include <sys/epoll.h>
#include <unistd.h>

namespace collie {

EPollPoller::~EPollPoller() noexcept {}

void EPollPoller::Init() noexcept {
  poll_fd_ = ::epoll_create1(0);
  if (poll_fd_ == -1) {
    throw;
  }
}

void EPollPoller::Destroy() noexcept {
  if (poll_fd_ == -1 || ::close(poll_fd_) == -1) {
    status_ = Error;
  } else {
    status_ = Ok;
    poll_fd_ = -1;
  }
}

void EPollPoller::Insert(const Descriptor& fd, Events events) noexcept {
  if (poll_fd_ == -1) {
    status_ = Error;
    return;
  }
  ::epoll_event e;
  e.data.fd = fd.GetDescriptor();
  e.events = events;
  if (::epoll_ctl(poll_fd_, EPOLL_CTL_ADD, e.data.fd, &e) == -1) {
    status_ = Error;
  } else {
    status_ = Ok;
  }
}

void EPollPoller::Update(const Descriptor& fd, Events events) noexcept {
  if (poll_fd_ == -1) {
    status_ = Error;
    return;
  }
  ::epoll_event e;
  e.data.fd = fd.GetDescriptor();
  e.events = events;
  if (::epoll_ctl(poll_fd_, EPOLL_CTL_MOD, e.data.fd, &e) == -1) {
    status_ = Error;
  } else {
    status_ = Ok;
  }
}
void EPollPoller::Delete(const Descriptor& fd) noexcept {
  if (poll_fd_ == -1) {
    status_ = Error;
    return;
  }
  if (::epoll_ctl(poll_fd_, EPOLL_CTL_DEL, fd.GetDescriptor(), nullptr) == -1) {
    status_ = Error;
  } else {
    status_ = Ok;
  }
}

void EPollPoller::Poll(PollHandler& handler, int timeout) noexcept {
  if (poll_fd_ == -1) {
    status_ = Error;
    return;
  }

  int size = max_event_num_;
  ::epoll_event revents[size];
  int num = ::epoll_wait(poll_fd_, revents, max_event_num_, timeout);
  if (num == -1) {
    status_ = Error;
    return;
  }

  for (int i = 0; i < num; ++i) {
    ::epoll_event& e = revents[i];
    handler(e.data.fd, e.events);
  }
  status_ = Ok;
}
}
