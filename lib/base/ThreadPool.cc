#include "../../inc/base/ThreadPool.h"

namespace collie {
namespace base {

void ThreadPool::Start() noexcept {
  if (!is_running) {
    is_running = true;
    try {
      for (unsigned i = 0; i < thread_num; ++i) {
        workers.emplace_back(&ThreadPool::Worker, this, i);
      }
    } catch (...) {
      // TODO
    }
  } else {
    // it is running
  }
}

void ThreadPool::Stop() noexcept {
  if (is_running) {
    is_running = false;
    Clear();
    worker_condition.notify_all();
    for (auto& worker : workers) {
      worker.join();
    }
  }
}

void ThreadPool::Push(Task&& task) noexcept {
  std::lock_guard<std::mutex> lock(worker_mutex);
  tasks.emplace(task);
  worker_condition.notify_all();
}

void ThreadPool::Clear() noexcept {
  std::lock_guard<std::mutex> lock(worker_mutex);
  decltype(tasks) empty_queue;
  std::swap(empty_queue, tasks);
}

void ThreadPool::Worker(const ThreadId&) {
  while (is_running) {
    Task task = nullptr;
    {
      std::unique_lock<std::mutex> lock(worker_mutex);
      if (tasks.empty())
        worker_condition.wait(lock);  // 不使用循环等待, 条件有可能为STOP信号
      if (tasks.empty()) continue;
      task = tasks.front();
      tasks.pop();
    }
    try {
      task();
    } catch (...) {
      // TODO
    }
  }
}
}
}
