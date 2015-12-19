#ifndef COLLIE_POLL_EPOLLPOLLER_H_
#define COLLIE_POLL_EPOLLPOLLER_H_

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include "poller.h"

namespace collie {
namespace poll {

// I/O multiplexing poller using epoll(Linux)
class EPollPoller : public Poller {
 public:
  using Event = struct epoll_event;
  enum class EventType : unsigned {
    READ = EPOLLIN,
    WRITE = EPOLLOUT,
    ERROR = EPOLLERR,
    CLOSE = EPOLLHUP,
    ONE_SHOT = EPOLLONESHOT,
    EDGE_TRIGGERED = EPOLLET,
    PRIORITY = EPOLLPRI
  };

  explicit EPollPoller(const unsigned maxEvent);
  EPollPoller(const EPollPoller &) = delete;
  EPollPoller &operator=(const EPollPoller &) = delete;
  ~EPollPoller() override;

  void Create() override;
  void Close() override;
  void Insert(const int fd, const unsigned events) override;
  void Modify(const int fd, const unsigned events) override;
  void Remove(const int fd) override;
  void Poll(PollCallback cb, const int timeout = -1) override;

  void EnableRead(unsigned &events) const noexcept override {
    events |= (unsigned)EventType::READ;
  }
  void DisableRead(unsigned &events) const noexcept override {
    events &= ~(unsigned)EventType::READ;
  }
  void EnableWrite(unsigned &events) const noexcept override {
    events |= (unsigned)EventType::WRITE;
  }
  void DisableWrite(unsigned &events) const noexcept override {
    events &= ~(unsigned)EventType::WRITE;
  }
  void EnableOneShot(unsigned &events) const noexcept override {
    events |= (unsigned)EventType::ONE_SHOT;
  }
  void DisableOneShot(unsigned &events) const noexcept override {
    events &= ~(unsigned)EventType::ONE_SHOT;
  }
  bool IsRead(const unsigned events) const noexcept override {
    return events & (unsigned)EventType::READ;
  }
  bool IsWrite(const unsigned events) const noexcept override {
    return events & (unsigned)EventType::WRITE;
  }
  bool IsError(const unsigned events) const noexcept override {
    return events & (unsigned)EventType::ERROR;
  }
  bool IsClose(const unsigned events) const noexcept override {
    return events & (unsigned)EventType::CLOSE;
  }

  // const unsigned MaxEvent;
 private:
  void CreateImpl() noexcept;
  void CloseImpl() noexcept;

  std::unique_ptr<Event> revents_;
};
}
}

#endif /* COLLIE_POLL_EPOLLPOLLER_H_ */
