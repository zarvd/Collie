#ifndef COLLIE_POLL_EVENT_POOL_H_
#define COLLIE_POLL_EVENT_POOL_H_

#include <memory>
#include <unordered_map>

#ifdef __linux__
#include "epoll_poller.h"
#define POLLER EPollPoller
#else
#include "select_poller.h"
#define POLLER SelectPoller
#endif

namespace collie {

class AsyncIOStream;

// should be shared pointer
class EventPool : public util::NonCopyable,
                  public std::enable_shared_from_this<EventPool> {
 public:
  EventPool() noexcept;
  ~EventPool() noexcept;

  void Loop(int timeout = -1) throw(PollException);
  void LoopOne(int timeout = -1) throw(PollException);
  void Init() throw(PollException);
  void Destroy() throw(PollException);

  void Update(std::shared_ptr<AsyncIOStream>) throw(PollException);
  void Delete(std::shared_ptr<AsyncIOStream>) throw(PollException);

  unsigned GetIONum() const noexcept { return io_streams_.size(); }
  bool IsEmpty() const noexcept { return io_streams_.empty(); }

 private:
  void PollHandler(unsigned fd, Event revents) noexcept;

  std::unique_ptr<Poller> poller_;

  std::unordered_map<int, std::shared_ptr<AsyncIOStream> > io_streams_;
};
}

#endif /* COLLIE_POLL_EVENT_POOL_H_ */
