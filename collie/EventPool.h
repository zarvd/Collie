#pragma once

#include <memory>
#include <unordered_map>

#ifdef __linux__
#include "EPollPoller.h"
#define POLLER EPollPoller
#else
#include "SelectPoller.h"
#define POLLER SelectPoller
#endif

namespace collie {

class AsyncIOStream;

class EventPool : public NonCopyable,
                  public std::enable_shared_from_this<EventPool> {
 public:
  EventPool();
  ~EventPool();

  void Loop(int timeout = -1);
  void LoopOne(int timeout = -1);

  void Update(std::shared_ptr<AsyncIOStream>);
  void Remove(std::shared_ptr<AsyncIOStream>);

  unsigned Size() const noexcept { return io_streams.size(); }
  bool IsEmpty() const noexcept { return io_streams.empty(); }

 private:
  void PollHandler(unsigned fd, EventType revents) noexcept;

  std::unique_ptr<Poller> poller;

  std::unordered_map<int, std::shared_ptr<AsyncIOStream> > io_streams;
};
}
