#ifndef COLLIE_BASE_THREAD_POOL_H_
#define COLLIE_BASE_THREAD_POOL_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "../collie.h"
#include "../util/NonCopyable.h"

namespace collie {
namespace base {

class ThreadPool : public util::NonCopyable {
 public:
  using Task = std::function<void()>;
  using ThreadId = unsigned;

  ThreadPool(const unsigned thread_num) noexcept : thread_num(thread_num) {}
  ~ThreadPool() noexcept;

  void Start() noexcept;
  void Stop() noexcept;

  void Restart() noexcept {
    Stop();
    Start();
  }

  void Push(Task&& task) noexcept;

  template <typename Functor, typename... Args>
  void Push(Functor&& functor, Args&&... args) noexcept {
    Push(std::bind(functor, std::forward<Args>(args)...));
  }

  void Clear() noexcept;

  void SetThreadNum(const unsigned thread_num) noexcept {
    this->thread_num = thread_num;
  }
  bool IsRunning() const noexcept { return is_running; }
  SizeType ThreadNum() const noexcept { return thread_num; }

 private:
  void Worker(const ThreadId&);  // working thread function

  unsigned thread_num;
  std::vector<std::thread> workers;
  std::mutex worker_mutex;
  std::condition_variable worker_condition;
  std::atomic<bool> is_running;
  std::queue<Task> tasks;
};
}
}

#endif /* COLLIE_BASE_THREAD_POOL_H_ */
