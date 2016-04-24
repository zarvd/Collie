#ifndef COLLIE_POLL_POLLER_H_
#define COLLIE_POLL_POLLER_H_

#include <memory>
#include <atomic>
#include "../util/noncopyable.h"
#include "../util/descriptor.h"

namespace collie {

class Poller : public NonCopyable, public Descriptor {
 public:
  using Events = unsigned;
  enum Status { NotInit, Ok, Error };
  using PollHandler = std::function<void(unsigned fd, Events revents)>;

  Poller() {}
  virtual ~Poller() noexcept = 0;

  virtual void Init() noexcept = 0;
  virtual void Destroy() noexcept = 0;
  // The timeout argument specifies the number of milliseconds that
  // Poller will block.
  // Specifying a timeout of -1 causes Poller to block indefinitely,
  // while specifying a timeout equal to zero cause Poller to return
  // immediately,
  // even if no events are available.
  virtual void Poll(PollHandler&, int timeout) noexcept = 0;
  virtual void Insert(const Descriptor&, Events) noexcept = 0;
  virtual void Update(const Descriptor&, Events) noexcept = 0;
  virtual void Delete(const Descriptor&) noexcept = 0;
  int GetDescriptor() const noexcept override { return poll_fd_; }

  Status status() const { return status_; }
  unsigned max_event_num() const { return max_event_num_; }
  void set_max_event_num(unsigned num) { max_event_num_ = num; }

 protected:
  int poll_fd_;
  Status status_;
  std::atomic<unsigned> max_event_num_;
};
}

#endif /* COLLIE_POLL_POLLER_H_ */
