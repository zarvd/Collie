#include "../inc/thread_pool.h"

namespace collie {

void ThreadPool::Start() noexcept {
  if (!is_running_) {
    is_running_ = true;
    try {
      for (unsigned i = 0; i < thread_num_; ++i) {
        workers_.emplace_back(&ThreadPool::worker, this, i);
      }
    } catch (...) {
      // TODO
    }
  } else {
    // it is running
  }
}

void ThreadPool::Stop() noexcept {
  if (is_running_) {
    is_running_ = false;
    ClearTask();
    worker_condition_.notify_all();
    for (auto& worker : workers_) {
      worker.join();
    }
  }
}

void ThreadPool::PushTask(Task&& task) noexcept {
  std::lock_guard<std::mutex> lock(worker_mutex_);
  tasks_.emplace(task);
  worker_condition_.notify_all();
}

void ThreadPool::ClearTask() noexcept {
  std::lock_guard<std::mutex> lock(worker_mutex_);
  decltype(tasks_) empty_queue;
  std::swap(empty_queue, tasks_);
}

void ThreadPool::worker(const ThreadId&) {
  while (is_running_) {
    Task task = nullptr;
    {
      std::unique_lock<std::mutex> lock(worker_mutex_);
      if (tasks_.empty())
        worker_condition_.wait(lock);  // 不使用循环等待, 条件有可能为STOP信号
      if (tasks_.empty()) continue;
      task = tasks_.front();
      tasks_.pop();
    }
    try {
      task();
    } catch (...) {
      // TODO
    }
  }
}
}
