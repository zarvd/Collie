#ifndef COLLIE_TIMER_H_
#define COLLIE_TIMER_H_

#include <functional>
#include <chrono>
#include <thread>
#include <queue>  // storing timeout handler
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>
#include "util/noncopyable.h"

namespace collie {

using TimeoutHandler = std::function<void()>;
enum class TimerPolicy { Once, Reapeat };

class Timer : public util::NonCopyable {
 public:
  enum Status { Timeout, Waiting, Removed };
  ~Timer() {}

  TimerPolicy GetPolicy() const { return policy; }
  Status GetStatus() const { return status; }

 private:
  Timer() : status(Waiting) {}

  std::chrono::high_resolution_clock::time_point timeout_point;
  TimeoutHandler handler;
  TimerPolicy policy;
  std::atomic<Status> status;

  friend class TimerQueue;
  friend struct TimerCmpGreater;
};

struct TimerCmpGreater {
  bool operator()(const Timer* x, const Timer* y) {
    return x->timeout_point > y->timeout_point;
  }
};

// Singleton
class TimerQueue : public util::NonCopyable {
 public:
  using TimeUnit = std::chrono::milliseconds;
  using TimePoint = std::chrono::high_resolution_clock::time_point;
  enum Status { Running, Stopped };

  ~TimerQueue() noexcept;

  // Thread safe
  void Start() noexcept;
  // Thread safe
  void Stop() noexcept;
  // Thread safe
  void SetFrequecy(TimeUnit&& frequency) noexcept { this->frequency = frequency; }
  const TimeUnit GetFrequency() const noexcept { return frequency; }
  // Thread safe
  Timer* Insert(const TimeUnit&, const TimeoutHandler&,
                const TimerPolicy = TimerPolicy::Once) noexcept;
  // Thread safe
  void Remove(Timer* const) noexcept;

  // Thread safe
  void Clear() noexcept;

  // Thread safe
  static TimerQueue& GetInstance() noexcept {
    static TimerQueue instance;
    return instance;
  }

  unsigned GetTimerNum() const noexcept { return queue.size(); }
  Status GetStatus() const noexcept { return status; }

 private:
  TimerQueue() noexcept;

  void Counter();
  std::mutex counter_mutex;
  std::thread counter;
  std::priority_queue<Timer*, std::vector<Timer*>, TimerCmpGreater>
      queue;  // min-heap
  Status status;
  std::atomic<TimeUnit> frequency;
};
}

#endif /* COLLIE_TIMER_H_ */
