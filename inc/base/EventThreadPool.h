#ifndef COLLIE_BASE_EVENT_THREAD_POOL_H_
#define COLLIE_BASE_EVENT_THREAD_POOL_H_

#include <atomic>
#include <thread>
#include <vector>
#include "../collie.h"
#include "../util/NonCopyable.h"

namespace collie {

class AsyncIOStream;
class EventPool;

class EventThreadPool : public util::NonCopyable {
 public:
  using IOStream = std::shared_ptr<AsyncIOStream>;

  EventThreadPool(unsigned thread_num) noexcept;
  ~EventThreadPool() noexcept;

  void Start() noexcept;
  void Stop() noexcept;
  void Push(IOStream) noexcept;      // push io into CURRENT event pool
  void PushInit(IOStream) noexcept;  // for init iostream

  SizeType ThreadNum() const noexcept { return thread_num; }
  void SetThreadNum(unsigned num) noexcept { thread_num = num; }
  bool IsRunning() const { return is_running; }
  std::shared_ptr<EventPool> CurrentEventPool() const noexcept {
    return current_event_pool;
  }

 private:
  void EventLoop();
  static thread_local std::shared_ptr<EventPool>
      current_event_pool;  // event pool in current thread

  std::atomic<bool> is_running;
  unsigned thread_num;
  std::vector<std::thread> workers;

  std::vector<IOStream> init_io_streams;
};
}

#endif /* COLLIE_BASE_EVENT_THREAD_POOL_H_ */
