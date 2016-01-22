#ifndef COLLIE_EVENT_EPOLLER_H_
#define COLLIE_EVENT_EPOLLER_H_

#include <sys/epoll.h>
#include <map>
#include <functional>
#include <memory>
#include "../descriptor.h"

namespace collie {
namespace event {

// I/O multiplexing poller using epoll(Linux)
class EPoller : public Descriptor {
public:
  using PollCallback =
      std::function<void(const int fd, const unsigned revents)>;
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

  explicit EPoller(const unsigned maxEvent);
  EPoller(const EPoller &) = delete;
  EPoller &operator=(const EPoller &) = delete;
  ~EPoller() override;

  void Create() override;
  void Close() override;
  void Insert(const int fd, const unsigned events);
  void Modify(const int fd, const unsigned events);
  void Remove(const int fd);
  void Poll(PollCallback cb, const int timeout = -1);

  static void EnableRead(unsigned &events) noexcept {
    events |= (unsigned)EventType::READ;
  }
  static void DisableRead(unsigned &events) noexcept {
    events &= ~(unsigned)EventType::READ;
  }
  static void EnableWrite(unsigned &events) noexcept {
    events |= (unsigned)EventType::WRITE;
  }
  static void DisableWrite(unsigned &events) noexcept {
    events &= ~(unsigned)EventType::WRITE;
  }
  static void EnableOneShot(unsigned &events) noexcept {
    events |= (unsigned)EventType::ONE_SHOT;
  }
  static void DisableOneShot(unsigned &events) noexcept {
    events &= ~(unsigned)EventType::ONE_SHOT;
  }
  static bool IsRead(const unsigned events) noexcept {
    return events & (unsigned)EventType::READ;
  }
  static bool IsWrite(const unsigned events) noexcept {
    return events & (unsigned)EventType::WRITE;
  }
  static bool IsError(const unsigned events) noexcept {
    return events & (unsigned)EventType::ERROR;
  }
  static bool IsClose(const unsigned events) noexcept {
    return events & (unsigned)EventType::CLOSE;
  }

  const unsigned kMaxEvent;

private:
  void CreateImpl() noexcept;
  void CloseImpl() noexcept;

  std::unique_ptr<Event> revents_;
};
}
}

#endif /* COLLIE_EVENT_EPOLLER_H_ */
