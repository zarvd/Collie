#ifndef COLLIE_POLL_POLLER_H_
#define COLLIE_POLL_POLLER_H_

#include <memory>
#include <atomic>
#include <string.h>
#include <functional>
#include "../util/noncopyable.h"
#include "../util/descriptor.h"
#include "../exception.h"

namespace collie {

class Event;

class Poller : public util::NonCopyable, public Descriptor {
 public:
  using PollHandler = std::function<void(unsigned fd, const Event& revents)>;

  Poller() : poll_fd_(-1), max_event_num_(200000) {}
  virtual ~Poller() noexcept = 0;

  virtual void Init() throw(PollException) = 0;
  virtual void Destroy() throw(PollException) = 0;
  // The timeout argument specifies the number of milliseconds that
  // Poller will block.
  // Specifying a timeout of -1 causes Poller to block indefinitely,
  // while specifying a timeout equal to zero cause Poller to return
  // immediately,
  // even if no events are available.
  virtual void Poll(const PollHandler&,
                    const int timeout) throw(PollException) = 0;
  virtual void Insert(unsigned fd, const Event&) throw(PollException) = 0;
  virtual void Update(unsigned fd, const Event&) throw(PollException) = 0;
  virtual void Delete(unsigned fd) throw(PollException) = 0;
  int GetDescriptor() const noexcept override { return poll_fd_; }

  unsigned max_event_num() const noexcept { return max_event_num_; }
  void set_max_event_num(unsigned num) noexcept { max_event_num_ = num; }

 protected:
  int poll_fd_;
  std::atomic<unsigned> max_event_num_;
};
}

#endif /* COLLIE_POLL_POLLER_H_ */
