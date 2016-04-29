#ifndef COLLIE_POLL_EPOLL_POLLER_H_
#define COLLIE_POLL_EPOLL_POLLER_H_

#include "poller.h"

namespace collie {

class EPollPoller : public Poller {
 public:
  EPollPoller();
  ~EPollPoller() noexcept override;

  void Init() throw(PollException) override;
  void Destroy() throw(PollException) override;
  void Poll(const PollHandler&,
            const int timeout) throw(PollException) override;
  void Insert(unsigned fd, const Event&) throw(PollException) override;
  void Update(unsigned fd, const Event&) throw(PollException) override;
  void Delete(unsigned fd) throw(PollException) override;

  static unsigned GetEvents(const Event&) noexcept;
  static Event GetEvent(unsigned events) noexcept;

 private:
};
}

#endif /* COLLIE_POLL_EPOLL_POLLER_H_ */
