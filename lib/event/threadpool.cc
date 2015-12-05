#include "../../include/event/threadpool.h"
#include "../../include/event/eventloop.h"
#include "../../include/event/channel.h"
#include "../../include/logging.h"

namespace collie {
namespace event {

ThreadPool::ThreadPool(const size_t thread_num)
    : kThreadNum(thread_num), terminate_(false) {}

ThreadPool::~ThreadPool() {
  if (!terminate_) Terminate();
}

void ThreadPool::Run() {
  // create thread
  for (size_t i = 0; i < kThreadNum; ++i) {
    workers_.emplace_back(&ThreadPool::RunInThread, this);
  }
}

void ThreadPool::Terminate() {
  {
    // send terminate signal to threads
    std::lock_guard<std::mutex> lock(mtx_);

    terminate_ = true;
  }
  for (auto& worker : workers_) {
    worker.join();
  }
}

void ThreadPool::RunInThread() {
  while (true) {
    Task task;
    {
      std::unique_lock<std::mutex> lock(mtx_);
      condition_.wait(
          lock, [this] { return this->terminate_ || !this->tasks_.empty(); });

      if (terminate_ && tasks_.empty()) return;  // exit
      task = std::move(tasks_.front());
      tasks_.pop();
    }
    task();
  }
}
}
}
