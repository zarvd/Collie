#include "../../inc/base/EPollPoller.h"
#include <sys/epoll.h>
#include <unistd.h>
#include "../../inc/base/EventType.h"
#include "../../inc/base/Logger.h"

namespace collie {

EPollPoller::EPollPoller() : Poller() {}
EPollPoller::~EPollPoller() noexcept {}

void EPollPoller::Init() {
  ASSERT(fd == -1)

  fd = ::epoll_create1(0);
  if (fd == -1) throw std::system_error();
}

void EPollPoller::Destroy() {
  ASSERT(fd != -1)

  if (::close(fd) == -1) throw std::system_error();
  fd = -1;
}

void EPollPoller::Insert(unsigned fd, const EventType& events) {
  ASSERT(this->fd != -1)

  ::epoll_event e;
  e.data.fd = fd;
  e.events = ToEvents(events);
  if (::epoll_ctl(this->fd, EPOLL_CTL_ADD, e.data.fd, &e) == -1) {
    throw std::system_error();
  }
}

void EPollPoller::Update(unsigned fd, const EventType& events) {
  ASSERT(this->fd != -1)

  ::epoll_event e;
  e.data.fd = fd;
  e.events = ToEvents(events);
  if (::epoll_ctl(this->fd, EPOLL_CTL_MOD, e.data.fd, &e) == -1) {
    throw std::system_error();
  }
}

void EPollPoller::Delete(unsigned fd) {
  ASSERT(this->fd != -1)

  if (::epoll_ctl(this->fd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
    throw std::system_error();
  }
}

void EPollPoller::Poll(const PollHandler& handler, int timeout) {
  ASSERT(this->fd != -1)

  int size = max_event;
  ::epoll_event revents[size];
  int num = ::epoll_wait(this->fd, revents, max_event, timeout);
  if (num == -1) {
    throw std::system_error();
  }

  for (int i = 0; i < num; ++i) {
    ::epoll_event& e = revents[i];
    handler(e.data.fd, ToEventType(e.events));
  }
}

unsigned EPollPoller::ToEvents(const EventType& event) noexcept {
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

EventType EPollPoller::ToEventType(unsigned events) noexcept {
  EventType event;
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
