#ifndef COLLIE_POLL_EVENT_THREAD_POOL_H_
#define COLLIE_POLL_EVENT_THREAD_POOL_H_

#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "../util/noncopyable.h"

namespace collie {

class AsyncIOStream;
class EventPool;

class EventThreadPool : public NonCopyable {
 public:
  using IOStream = std::shared_ptr<AsyncIOStream>;

  EventThreadPool(unsigned thread_num) noexcept;
  ~EventThreadPool() noexcept;

  void Start();
  void Stop();
  void Push(IOStream) noexcept;      // push io into CURRENT event pool
  void PushInit(IOStream) noexcept;  // for init iostream

  unsigned thread_num() const noexcept { return thread_num_; }
  void set_thread_num(unsigned num) noexcept { thread_num_ = num; }
  bool is_running() const { return is_running_; }

  static thread_local std::shared_ptr<EventPool>
      CurrentEventPool;  // event pool in current thread

 private:
  void EventLoop();

  std::atomic<bool> is_running_;
  unsigned thread_num_;
  std::vector<std::thread> workers_;

  std::vector<IOStream> init_ios_;
};
}

#endif /* COLLIE_POLL_EVENT_THREAD_POOL_H_ */
