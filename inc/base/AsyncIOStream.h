#ifndef COLLIE_BASE_ASYNC_IO_STREAM_H_
#define COLLIE_BASE_ASYNC_IO_STREAM_H_

#include <memory>
#include "../util/NonCopyable.h"
#include "EventType.h"

namespace collie {
namespace base {

class EventPool;

class AsyncIOStream : public util::NonCopyable {
 public:
  constexpr AsyncIOStream() noexcept {}
  virtual ~AsyncIOStream() noexcept {};

  virtual int Descriptor() const noexcept = 0;
  virtual void HandleRead() noexcept = 0;
  virtual void HandleWrite() noexcept = 0;
  virtual void HandleClose() noexcept = 0;
  virtual void HandleError() noexcept = 0;
  EventType Event() const noexcept { return this->event; }
  void SetEvent(const EventType& event) noexcept { this->event = event; }
  std::shared_ptr<EventPool> CurrentEventPool() const noexcept {
    return event_pool;
  }

  friend class EventPool;

 protected:
  EventType event;
  std::shared_ptr<EventPool> event_pool;
};
}
}

#endif /* COLLIE_BASE_ASYNC_IO_STREAM_H_ */
