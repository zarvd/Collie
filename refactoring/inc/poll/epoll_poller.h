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
  void Poll(PollHandler&, const int timeout) throw(PollException) override;
  void Insert(const Descriptor&, const Events) throw(PollException) override;
  void Update(const Descriptor&, const Events) throw(PollException) override;
  void Delete(const Descriptor&) throw(PollException) override;

 private:
};
}

#endif /* COLLIE_POLL_EPOLL_POLLER_H_ */
