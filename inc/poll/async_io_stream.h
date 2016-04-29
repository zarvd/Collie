#ifndef COLLIE_ASYNC_IO_STREAM_H_
#define COLLIE_ASYNC_IO_STREAM_H_

#include "../util/noncopyable.h"
#include "../util/descriptor.h"
#include "event.h"
#include <memory>

namespace collie {

class EventPool;

class AsyncIOStream : public NonCopyable, public Descriptor {
 public:
  AsyncIOStream();
  virtual ~AsyncIOStream() = 0;

  virtual int GetDescriptor() const noexcept = 0;
  virtual void HandleRead() noexcept = 0;
  virtual void HandleWrite() noexcept = 0;
  virtual void HandleClose() noexcept = 0;
  virtual void HandleError() noexcept = 0;
  Event event() const noexcept { return event_; }
  std::shared_ptr<EventPool> event_pool() const noexcept { return event_pool_; }

  friend class EventPool;

 protected:
  Event event_;
  std::shared_ptr<EventPool> event_pool_;
};
}

#endif /* COLLIE_ASYNC_IO_STREAM_H_ */
