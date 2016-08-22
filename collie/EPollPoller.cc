#include <collie/EPollPoller.h>
#include <collie/Logger.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

namespace collie {

EPollPoller::EPollPoller() : efd(::epoll_create1(0)) {
  if (efd == -1) throw std::runtime_error(::strerror(errno));
}
EPollPoller::~EPollPoller() { ::close(efd); }

void EPollPoller::Insert(const unsigned fd, const EventType& events) const {
  ::epoll_event e;
  e.data.fd = fd;
  e.events = ToEvents(events);
  const int ret = ::epoll_ctl(efd, EPOLL_CTL_ADD, e.data.fd, &e);
  if (ret == -1) throw std::runtime_error(::strerror(errno));
}

void EPollPoller::Update(const unsigned fd, const EventType& events) const {
  ::epoll_event e;
  e.data.fd = fd;
  e.events = ToEvents(events);
  const int ret = ::epoll_ctl(efd, EPOLL_CTL_MOD, e.data.fd, &e);
  if (ret == -1) throw std::runtime_error(::strerror(errno));
}

void EPollPoller::Remove(const unsigned fd) const {
  ASSERT(efd != -1)

  const int ret = ::epoll_ctl(efd, EPOLL_CTL_DEL, fd, nullptr);
  if (ret == -1) throw std::runtime_error(::strerror(errno));
}

void EPollPoller::Poll(const PollCallback& cb, const int timeout,
                       int max_event) const {
  LOG(DEBUG) << "POLLING";
  if (max_event == -1) max_event = 1000;
  ::epoll_event revents[max_event];
  const int ret = ::epoll_wait(efd, revents, max_event, timeout);

  if (ret == -1) throw std::runtime_error(::strerror(errno));

  for (int i = 0; i < ret; ++i) {
    const auto& e = revents[i];
    cb(e.data.fd, ToEventType(e.events));
  }
}

unsigned EPollPoller::ToEvents(const EventType& event) const noexcept {
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

EventType EPollPoller::ToEventType(const unsigned events) const noexcept {
  EventType event;
  event.SetRead((events & EPOLLIN) == EPOLLIN);
  event.SetWrite((events & EPOLLOUT) == EPOLLOUT);
  event.SetClose((events & EPOLLRDHUP) == EPOLLRDHUP);
  event.SetError((events & EPOLLERR) == EPOLLERR);
  event.SetEdgeTriggeder((events & EPOLLET) == EPOLLET);
  event.SetUrgetnRead((events & EPOLLPRI) == EPOLLPRI);
  event.SetOneShot((events & EPOLLONESHOT) == EPOLLONESHOT);
  return event;
}
}
