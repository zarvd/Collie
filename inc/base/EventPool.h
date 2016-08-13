#ifndef COLLIE_BASE_EVENT_POOL_H_
#define COLLIE_BASE_EVENT_POOL_H_

#include <memory>
#include <unordered_map>
#include "../collie.h"

#ifdef __linux__
#include "EPollPoller.h"
#define POLLER EPollPoller
#else
#include "SelectPoller.h"
#define POLLER SelectPoller
#endif

namespace collie {

class AsyncIOStream;

class EventPool : public util::NonCopyable,
                  public std::enable_shared_from_this<EventPool> {
 public:
  EventPool();
  ~EventPool();

  void Loop(int timeout = -1);
  void LoopOne(int timeout = -1);

  void Update(std::shared_ptr<AsyncIOStream>);
  void Remove(std::shared_ptr<AsyncIOStream>);

  SizeType Size() const noexcept { return io_streams.size(); }
  bool IsEmpty() const noexcept { return io_streams.empty(); }

 private:
  void PollHandler(unsigned fd, EventType revents) noexcept;

  std::unique_ptr<Poller> poller;

  std::unordered_map<int, std::shared_ptr<AsyncIOStream> > io_streams;
};
}

#endif /* COLLIE_BASE_EVENT_POOL_H_ */
