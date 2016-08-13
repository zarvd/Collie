#ifndef COLLIE_BASE_POLLER_H_
#define COLLIE_BASE_POLLER_H_

#include <functional>
#include "../util/NonCopyable.h"
#include "EventType.h"

namespace collie {

class Poller : public util::NonCopyable {
 public:
  using PollCallback = std::function<void(const unsigned, const EventType&)>;

  constexpr Poller() {}
  virtual ~Poller() = 0;

  // The timeout argument specifies the number of milliseconds that
  // Poller will block.
  // Specifying a timeout of -1 causes Poller to block indefinitely,
  // while specifying a timeout equal to zero cause Poller to return
  // immediately,
  // even if no events are available.
  virtual void Poll(const PollCallback&, const int timeout = -1,
                    int max_event = -1) const = 0;

  virtual void Insert(const unsigned fd, const EventType&) const = 0;
  virtual void Update(const unsigned fd, const EventType&) const = 0;
  virtual void Remove(const unsigned fd) const = 0;

  virtual unsigned MaxEvent() const noexcept = 0;
  virtual void SetMaxEvent(unsigned num) noexcept = 0;

  virtual unsigned ToEvents(const EventType&) const noexcept = 0;
  virtual EventType ToEventType(const unsigned events) const noexcept = 0;
};
}

#endif /* COLLIE_BASE_POLLER_H_ */
