#ifndef COLLIE_BASE_TIMER_H_
#define COLLIE_BASE_TIMER_H_

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "../util/noncopyable.h"

namespace collie {
namespace base {

using TimeoutHandler = std::function<void()>;
enum class TimerPolicy { Once, Reapeat };

template <class TimeUnit>
class TimerQueue;

template <class TimeUnit>
struct TimerCmpGreater;

// Timer
template <class TimeUnit = std::chrono::milliseconds>
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

  friend class TimerQueue<TimeUnit>;
  friend struct TimerCmpGreater<TimeUnit>;
};

// Compare timer
template <class TimeUnit = std::chrono::milliseconds>
struct TimerCmpGreater {
  using TimerPtr = std::shared_ptr<Timer<TimeUnit> >;

  bool operator()(const TimerPtr x, const TimerPtr y) {
    return x->timeout_point > y->timeout_point;
  }
};

// Timer queue
template <class TimeUnit = std::chrono::milliseconds>
class TimerQueue : public util::NonCopyable {
 public:
  // using TimeUnit = std::chrono::milliseconds;
  using TimerPtr = std::shared_ptr<Timer<TimeUnit> >;
  using TimePoint = std::chrono::high_resolution_clock::time_point;
  enum Status { Running, Stopped };

  constexpr TimerQueue() noexcept;
  ~TimerQueue() noexcept;

  // Thread safe
  void Start() noexcept;
  // Thread safe
  void Stop() noexcept;
  // Thread safe
  void SetFrequecy(TimeUnit&& frequency) noexcept {
    this->frequency = frequency;
  }
  const TimeUnit GetFrequency() const noexcept { return frequency; }
  // Thread safe
  TimerPtr Insert(const TimeUnit&, const TimeoutHandler&,
                  const TimerPolicy = TimerPolicy::Once) noexcept;

  // Modifies timer if it's waiting
  // Inserts timer if it's timeout or removed
  TimerPtr Insert(TimerPtr) noexcept;
  // Thread safe
  void Remove(TimerPtr) noexcept;

  // Thread safe
  void Clear() noexcept;

  unsigned GetTimerNum() const noexcept { return queue.size(); }
  Status GetStatus() const noexcept { return status; }

 private:
  void Counter();
  std::mutex counter_mutex;
  std::thread counter;
  std::priority_queue<TimerPtr, std::vector<TimerPtr>,
                      TimerCmpGreater<TimeUnit> >
      queue;  // min-heap
  Status status;
  std::atomic<TimeUnit> frequency;
};
}
}

#endif /* COLLIE_BASE_TIMER_H_ */
