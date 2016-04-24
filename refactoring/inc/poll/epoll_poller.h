#ifndef COLLIE_POLL_EPOLL_POLLER_H_
#define COLLIE_POLL_EPOLL_POLLER_H_

#include <vector>
#include <functional>
#include "poller.h"

namespace collie {
class EPollPoller : public Poller {
 public:
  EPollPoller();
  ~EPollPoller() noexcept override;

  void Init() noexcept override;
  void Destroy() noexcept override;
  void Poll(PollHandler&, int timeout) noexcept override;
  void Insert(const Descriptor&, Events) noexcept override;
  void Update(const Descriptor&, Events) noexcept override;
  void Delete(const Descriptor&) noexcept override;

 private:
};
}

#endif /* COLLIE_POLL_EPOLL_POLLER_H_ */
