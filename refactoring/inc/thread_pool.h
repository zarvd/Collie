#ifndef COLLIE_THREAD_POOL_H_
#define COLLIE_THREAD_POOL_H_

#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <functional>
#include <thread>
#include "util/noncopyable.h"

namespace collie {

class ThreadPool : public NonCopyable {
 public:
  using Task = std::function<void()>;
  using ThreadId = unsigned;

  ThreadPool(const unsigned thread_num) noexcept : thread_num_(thread_num) {}
  ~ThreadPool();

  void Start() noexcept;
  void Stop() noexcept;

  void Restart() noexcept {
    Stop();
    Start();
  }

  void PushTask(Task&& task) noexcept;

  template <typename Functor, typename... Args>
  void PushTask(Functor&& functor, Args&&... args) noexcept {
    PushTask(std::bind(functor, std::forward<Args>(args)...));
  }

  void ClearTask() noexcept;

  void set_thread_num(const unsigned thread_num) noexcept {
    thread_num_ = thread_num;
  }
  bool is_running() const noexcept { return is_running_; }
  unsigned thread_num() const noexcept { return thread_num_; }

 private:
  void worker(const ThreadId&);  // working thread function

  unsigned thread_num_;
  std::vector<std::thread> workers_;
  std::mutex worker_mutex_;
  std::condition_variable worker_condition_;
  std::atomic<bool> is_running_;
  std::queue<Task> tasks_;
};
}

#endif /* COLLIE_THREAD_POOL_H_ */
