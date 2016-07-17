#ifndef COLLIE_BASE_POLLER_H_
#define COLLIE_BASE_POLLER_H_

#include <atomic>
#include <functional>
#include "../util/NonCopyable.h"

namespace collie {

class EventType;

class Poller : public util::NonCopyable {
 public:
  using PollHandler =
      std::function<void(unsigned fd, const EventType& revents)>;

  Poller() : fd(-1), max_event(200000) {}
  virtual ~Poller() noexcept = 0;

  virtual void Init() = 0;
  virtual void Destroy() = 0;
  // The timeout argument specifies the number of milliseconds that
  // Poller will block.
  // Specifying a timeout of -1 causes Poller to block indefinitely,
  // while specifying a timeout equal to zero cause Poller to return
  // immediately,
  // even if no events are available.
  virtual void Poll(const PollHandler&, const int timeout) = 0;
  virtual void Insert(unsigned fd, const EventType&) = 0;
  virtual void Update(unsigned fd, const EventType&) = 0;
  virtual void Delete(unsigned fd) = 0;

  unsigned MaxEvent() const noexcept { return max_event; }
  void SetMaxEvent(unsigned num) noexcept { max_event = num; }

 protected:
  int fd;
  std::atomic<unsigned> max_event;
};
}

#endif /* COLLIE_BASE_POLLER_H_ */
