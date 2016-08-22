#pragma once

#include <collie/EventPool.h>
#include <collie/NonCopyable.h>
#include <atomic>
#include <thread>
#include <unordered_map>
#include <vector>

namespace collie {

class EventThreadPool : public NonCopyable {
 public:
  explicit EventThreadPool(const unsigned thread_num) noexcept;
  ~EventThreadPool();

  void Start() noexcept;
  void Stop() noexcept;
  void Push(std::shared_ptr<AsyncIOStream>,
            bool to_all = false) noexcept;  // push io into CURRENT
                                            // event pool

  unsigned ThreadNum() const noexcept { return thread_num; }
  void SetThreadNum(unsigned num) noexcept { thread_num = num; }
  bool IsRunning() const { return is_running; }
  std::shared_ptr<EventPool> CurrentEventPool() const noexcept {
    auto it = event_pools.find(std::this_thread::get_id());
    if (it != event_pools.end()) return it->second;
    return nullptr;
  }

 private:
  void EventLoop();
  // static thread_local std::shared_ptr<EventPool>
  //     current_event_pool;  // event pool in current thread
  std::unordered_map<std::thread::id, std::shared_ptr<EventPool>> event_pools;

  std::atomic<bool> is_running;
  unsigned thread_num;
  std::vector<std::thread> workers;
};
}
