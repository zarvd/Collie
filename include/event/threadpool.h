#ifndef COLLIE_EVENT_THREADPOOL_H_
#define COLLIE_EVENT_THREADPOOL_H_

#include <functional>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <atomic>
#include "../exception.h"

namespace collie {
namespace event {

class ThreadPool {
 public:
  using Task = std::function<void()>;

  explicit ThreadPool(const size_t thread_num);
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ~ThreadPool();

  void Run();
  void Terminate();
  bool terminate() const { return terminate_; }

  template <class Cb, class... Args>
  auto Enqueue(Cb &&cb, Args &&... args)
      -> std::future<typename std::result_of<Cb(Args...)>::type>;

  const size_t kThreadNum;

 private:
  void RunInThread();

  std::mutex mtx_;
  std::condition_variable condition_;
  std::vector<std::thread> workers_;
  std::queue<Task> tasks_;

  std::atomic<bool> terminate_;
};

template <class Cb, class... Args>
auto ThreadPool::Enqueue(Cb &&cb, Args &&... args)
    -> std::future<typename std::result_of<Cb(Args...)>::type> {
  using returnType = typename std::result_of<Cb(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<returnType()>>(
      std::bind(std::forward<Cb>(cb), std::forward<Args>(args)...));

  std::future<returnType> res = task->get_future();
  {
    std::lock_guard<std::mutex> lock(mtx_);

    // don't allow enqueueing after stopping the pool
    if (terminate_) {
      THROW_("enqueue on stopped ThreadPool");
    }
    tasks_.emplace([task]() { (*task)(); });
  }
  condition_.notify_one();
  return res;
}
}
}

#endif /* COLLIE_EVENT_THREADPOOL_H_ */
