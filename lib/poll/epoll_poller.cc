#include "../../inc/poll/event.h"
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

void EPollPoller::Insert(unsigned fd,
                         const Event& events) throw(PollException) {
  ASSERT(poll_fd_ != -1)

  ::epoll_event e;
  e.data.fd = fd;
  e.events = GetEvents(events);
  if (::epoll_ctl(poll_fd_, EPOLL_CTL_ADD, e.data.fd, &e) == -1) {
    throw PollException("EPoll cannot insert");
  }
}

void EPollPoller::Update(unsigned fd,
                         const Event& events) throw(PollException) {
  ASSERT(poll_fd_ != -1)

  ::epoll_event e;
  e.data.fd = fd;
  e.events = GetEvents(events);
  if (::epoll_ctl(poll_fd_, EPOLL_CTL_MOD, e.data.fd, &e) == -1) {
    throw PollException("EPoll cannot update");
  }
}
void EPollPoller::Delete(unsigned fd) throw(PollException) {
  ASSERT(poll_fd_ != -1)

  if (::epoll_ctl(poll_fd_, EPOLL_CTL_DEL, fd, nullptr) == -1) {
    throw PollException("EPoll cannot delete");
  }
}

void EPollPoller::Poll(const PollHandler& handler,
                       int timeout) throw(PollException) {
  ASSERT(poll_fd_ != -1)

  int size = max_event_num_;
  ::epoll_event revents[size];
  int num = ::epoll_wait(poll_fd_, revents, max_event_num_, timeout);
  if (num == -1) {
    throw PollException("EPoll cannot poll");
  }

  for (int i = 0; i < num; ++i) {
    ::epoll_event& e = revents[i];
    handler(e.data.fd, GetEvent(e.events));
  }
}

unsigned EPollPoller::GetEvents(const Event& event) noexcept {
  unsigned events = 0;
  if (event.IsRead()) {
    events |= EPOLLIN;
  }
  if (event.IsWrite()) {
    events |= EPOLLOUT;
  }
  if (event.IsClose()) {
    events |= EPOLLRDHUP;
  }
  if (event.IsError()) {
    events |= EPOLLERR;
  }
  if (event.IsEdgeTriggerd()) {
    events |= EPOLLET;
  }
  if (event.IsUrgentRead()) {
    events |= EPOLLPRI;
  }
  if (event.IsOneShot()) {
    events |= EPOLLONESHOT;
  }
  return events;
}

Event EPollPoller::GetEvent(unsigned events) noexcept {
  Event event;
  if ((events & EPOLLIN) == EPOLLIN) {
    event.SetRead(true);
  }
  if ((events & EPOLLOUT) == EPOLLOUT) {
    event.SetWrite(true);
  }
  if ((events & EPOLLRDHUP) == EPOLLRDHUP) {
    event.SetClose(true);
  }
  if ((events & EPOLLERR) == EPOLLERR) {
    event.SetError(true);
  }
  if ((events & EPOLLET) == EPOLLET) {
    event.SetEdgeTriggeder(true);
  }
  if ((events & EPOLLPRI) == EPOLLPRI) {
    event.SetUrgetnRead(true);
  }
  if ((events & EPOLLONESHOT) == EPOLLONESHOT) {
    event.SetOneShot(true);
  }
  return event;
}
}
