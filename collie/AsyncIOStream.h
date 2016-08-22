#pragma once

#include <collie/EventPool.h>
#include <collie/EventType.h>
#include <collie/NonCopyable.h>
#include <memory>

namespace collie {

class AsyncIOStream : public NonCopyable {
 public:
  constexpr AsyncIOStream() noexcept {}
  virtual ~AsyncIOStream(){};

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
