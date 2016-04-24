#include "../inc/timer.h"

namespace collie {

TimerQueue::TimerQueue() noexcept : status_(Stopped) {}

TimerQueue::~TimerQueue() noexcept {
  if (status_ == Running) Stop();
}

void TimerQueue::Start() noexcept {
  if (status_ == Running) return;
  try {
    counter_ = std::thread(&TimerQueue::Counter, this);
  } catch (...) {
    // TODO
  }
  status_ = Running;
}

void TimerQueue::Stop() noexcept {
  if (status_ == Stopped) return;

  try {
    status_ = Stopped;
    counter_.join();
  } catch (...) {
    // TODO
  }
}

Timer *TimerQueue::Insert(const TimeUnit &time, const TimeoutHandler &handler,
                          const TimerPolicy policy) noexcept {
  if (status_ == Stopped) return nullptr;
  const auto now = std::chrono::high_resolution_clock::now();

  Timer *timer = new Timer;
  timer->timeout_point_ = now + time;
  timer->handler_ = handler;
  timer->policy_ = policy;

  {
    std::lock_guard<std::mutex> lock(counter_mutex_);
    queue_.push(timer);
  }
  return timer;
}

void TimerQueue::Remove(Timer *const timer) noexcept {
  if (status_ == Stopped) return;
  if (timer->status_ != Timer::Waiting) return;

  timer->status_ = Timer::Removed;
}

void TimerQueue::Clear() noexcept {
  std::lock_guard<std::mutex> lock(counter_mutex_);
  decltype(queue_) empty_queue;
  std::swap(empty_queue, queue_);
}

void TimerQueue::Counter() {
  while (status_ == Running) {
    TimeUnit frequency = frequency_;
    std::this_thread::sleep_for(frequency);
    const auto now = std::chrono::high_resolution_clock::now();
    while (true) {
      std::lock_guard<std::mutex> lock(counter_mutex_);
      if (queue_.empty()) break;

      // Get the closet timer
      Timer *timer = queue_.top();
      if (timer->timeout_point_ <= now) {
        if (timer->status_ == Timer::Waiting) {
          // Avaliable timer
          timer->handler_();
          timer->status_ = Timer::Timeout;
        }
        queue_.pop();
        continue;
      }
      // No Avaliable timers
      // Keep looping
      break;
    }
  }
}
}
