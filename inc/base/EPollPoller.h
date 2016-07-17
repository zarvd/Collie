#ifndef COLLIE_BASE_EPOLL_POLLER_H_
#define COLLIE_BASE_EPOLL_POLLER_H_

#include "Poller.h"

namespace collie {
namespace base {

class EPollPoller : public Poller {
 public:
  EPollPoller();
  ~EPollPoller() noexcept override;

  void Init() override;
  void Destroy() override;
  void Poll(const PollHandler&, const int timeout) override;
  void Insert(unsigned fd, const EventType&) override;
  void Update(unsigned fd, const EventType&) override;
  void Delete(unsigned fd) override;

  static unsigned ToEvents(const EventType&) noexcept;
  static EventType ToEventType(unsigned events) noexcept;

 private:
};
}
}

#endif /* COLLIE_BASE_EPOLL_POLLER_H_ */
