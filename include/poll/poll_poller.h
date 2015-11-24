#ifndef COLLIE_POLL_POLLPOLLER_H_
#define COLLIE_POLL_POLLPOLLER_H_

#include <memory>
#include "poller.h"

namespace collie {
namespace poll {

// TODO
class PollPoller : public Poller {
 public:
  explicit PollPoller(const unsigned &maxEvent);
  PollPoller(const PollPoller &) = delete;
  PollPoller &operator=(const PollPoller &) = delete;
  ~PollPoller();

  void Insert(const int fd, const unsigned events) override;
  void Modify(const int fd, const unsigned events) override;
  void Remove(const int fd) override;
  void Poll(PollCallback cb, const int timeout = -1) override;

  virtual void EnableRead(unsigned &events) const noexcept override;
  virtual void DisableRead(unsigned &events) const noexcept override;
  virtual void EnableWrite(unsigned &events) const noexcept override;
  virtual void DisableWrite(unsigned &events) const noexcept override;
  virtual bool IsRead(const unsigned events) const noexcept override;
  virtual bool IsWrite(const unsigned events) const noexcept override;
  virtual bool IsError(const unsigned events) const noexcept override;
  virtual bool IsClose(const unsigned events) const noexcept override;

 private:
  void Open() override;
};
}
}

#endif /* COLLIE_POLL_POLLPOLLER_H_ */
