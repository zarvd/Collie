#ifndef COLLIE_POLL_POLLER_H_
#define COLLIE_POLL_POLLER_H_

#include <functional>
#include "../descriptor.h"

namespace collie {
namespace poll {

/**
 * Abstract class
 */
class Poller : public Descriptor {
 public:
  using PollCallback =
      std::function<void(const int fd, const unsigned revents)>;

  explicit Poller(const unsigned maxEvent) noexcept : Descriptor(-1, false),
                                                      kMaxEvent(maxEvent) {}
  Poller(const Poller &) = delete;
  Poller &operator=(const Poller &) = delete;
  virtual ~Poller() = 0;

  virtual void Insert(const int fd, const unsigned events) = 0;
  virtual void Modify(const int fd, const unsigned events) = 0;
  virtual void Remove(const int fd) = 0;
  virtual void Poll(PollCallback cb, const int timeout = -1) = 0;

  virtual void EnableRead(unsigned &events) const noexcept = 0;
  virtual void DisableRead(unsigned &events) const noexcept = 0;
  virtual void EnableWrite(unsigned &events) const noexcept = 0;
  virtual void DisableWrite(unsigned &events) const noexcept = 0;
  virtual void EnableOneShot(unsigned &events) const noexcept = 0;
  virtual void DisableOneShot(unsigned &events) const noexcept = 0;
  virtual bool IsRead(const unsigned events) const noexcept = 0;
  virtual bool IsWrite(const unsigned events) const noexcept = 0;
  virtual bool IsError(const unsigned events) const noexcept = 0;
  virtual bool IsClose(const unsigned events) const noexcept = 0;

  const unsigned kMaxEvent;

 protected:
  void CloseImpl() noexcept override;
};
}
}

#endif /* COLLIE_POLL_POLLER_H_ */
