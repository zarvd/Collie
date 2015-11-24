#ifndef COLLIE_POLL_EPOLLPOLLER_H_
#define COLLIE_POLL_EPOLLPOLLER_H_

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include "poller.h"

namespace collie {
namespace poll {

class EPollPoller : public Poller {
 public:
  using Event = struct epoll_event;

  explicit EPollPoller(const unsigned maxEvent);
  EPollPoller(const EPollPoller &) = delete;
  EPollPoller &operator=(const EPollPoller &) = delete;
  ~EPollPoller() override;

  void Create() override;
  void Insert(const int fd, const unsigned events) override;
  void Modify(const int fd, const unsigned events) override;
  void Remove(const int fd) override;
  void Poll(PollCallback cb, const int timeout = -1) override;

  enum class Type : unsigned {
    Read = EPOLLIN,
    Write = EPOLLOUT,
    Error = EPOLLERR,
    Close = EPOLLHUP,
    OneShot = EPOLLONESHOT,
    EdgeTriggered = EPOLLET,
    Priority = EPOLLPRI
  };

  void EnableRead(unsigned &events) const noexcept override {
    events |= (unsigned)Type::Read;
  }
  void DisableRead(unsigned &events) const noexcept override {
    events &= ~(unsigned)Type::Read;
  }
  void EnableWrite(unsigned &events) const noexcept override {
    events |= (unsigned)Type::Write;
  }
  void DisableWrite(unsigned &events) const noexcept override {
    events &= ~(unsigned)Type::Write;
  }
  void EnableOneShot(unsigned &events) const noexcept override {
    events |= (unsigned)Type::OneShot;
  }
  void DisableOneShot(unsigned &events) const noexcept override {
    events &= ~(unsigned)Type::OneShot;
  }
  bool IsRead(const unsigned events) const noexcept override {
    return (unsigned)Type::Read & events;
  }
  bool IsWrite(const unsigned events) const noexcept override {
    return (unsigned)Type::Write & events;
  }
  bool IsError(const unsigned events) const noexcept override {
    return (unsigned)Type::Error & events;
  }
  bool IsClose(const unsigned events) const noexcept override {
    return (unsigned)Type::Close & events;
  }

  // const unsigned MaxEvent;
 private:
  bool is_init_;
  std::unique_ptr<Event> revents_;
};
}
}

#endif /* COLLIE_POLL_EPOLLPOLLER_H_ */
