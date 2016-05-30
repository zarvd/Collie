#include "../inc/timer.h"

namespace collie {

TimerQueue::TimerQueue() noexcept : status(Stopped) {}

TimerQueue::~TimerQueue() noexcept {
  if (status == Running) Stop();
}

void TimerQueue::Start() noexcept {
  if (status == Running) return;
  try {
    counter = std::thread(&TimerQueue::Counter, this);
  } catch (...) {
    // TODO
  }
  status = Running;
}

void TimerQueue::Stop() noexcept {
  if (status == Stopped) return;

  try {
    status = Stopped;
    counter.join();
  } catch (...) {
    // TODO
  }
}

Timer *TimerQueue::Insert(const TimeUnit &time, const TimeoutHandler &handler,
                          const TimerPolicy policy) noexcept {
  if (status == Stopped) return nullptr;
  const auto now = std::chrono::high_resolution_clock::now();

  Timer *timer = new Timer;
  timer->timeout_point = now + time;
  timer->handler = handler;
  timer->policy = policy;

  {
    std::lock_guard<std::mutex> lock(counter_mutex);
    queue.push(timer);
  }
  return timer;
}

void TimerQueue::Remove(Timer *const timer) noexcept {
  if (status == Stopped) return;
  if (timer->status != Timer::Waiting) return;

  timer->status = Timer::Removed;
}

void TimerQueue::Clear() noexcept {
  std::lock_guard<std::mutex> lock(counter_mutex);
  decltype(queue) empty_queue;
  std::swap(empty_queue, queue);
}

void TimerQueue::Counter() {
  while (status == Running) {
    TimeUnit frequency = this->frequency;
    std::this_thread::sleep_for(frequency);
    const auto now = std::chrono::high_resolution_clock::now();
    while (true) {
      std::lock_guard<std::mutex> lock(counter_mutex);
      if (queue.empty()) break;

      // Get the closet timer
      Timer *timer = queue.top();
      if (timer->timeout_point <= now) {
        if (timer->status == Timer::Waiting) {
          // Avaliable timer
          timer->handler();
          timer->status = Timer::Timeout;
        }
        queue.pop();
        continue;
      }
      // No Avaliable timers
      // Keep looping
      break;
    }
  }
}
}
