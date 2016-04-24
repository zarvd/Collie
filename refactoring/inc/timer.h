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

class Timer : public NonCopyable {
 public:
  enum Status { Timeout, Waiting, Removed };
  ~Timer() {}

  TimerPolicy policy() const { return policy_; }
  Status status() const { return status_; }

 private:
  Timer() : status_(Waiting) {}

  std::chrono::high_resolution_clock::time_point timeout_point_;
  TimeoutHandler handler_;
  TimerPolicy policy_;
  std::atomic<Status> status_;

  friend class TimerQueue;
  friend struct TimerCmpGreater;
};

struct TimerCmpGreater {
  bool operator()(const Timer* x, const Timer* y) {
    return x->timeout_point_ > y->timeout_point_;
  }
};

// Singleton
class TimerQueue : public NonCopyable {
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
  void SetFrequecy(TimeUnit&& frequency) noexcept { frequency_ = frequency; }
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

  unsigned GetTimerNum() const noexcept { return queue_.size(); }
  Status status() const noexcept { return status_; }
  TimeUnit frequency() const noexcept { return frequency_; }

 private:
  TimerQueue() noexcept;

  void Counter();
  std::mutex counter_mutex_;
  std::thread counter_;
  std::priority_queue<Timer*, std::vector<Timer*>, TimerCmpGreater>
      queue_;  // min-heap
  Status status_;
  std::atomic<TimeUnit> frequency_;
};
}

#endif /* COLLIE_TIMER_H_ */
