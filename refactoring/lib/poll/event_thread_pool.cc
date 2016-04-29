#include "../../inc/poll/async_io_stream.h"
#include "../../inc/poll/event_pool.h"
#include "../../inc/poll/event_thread_pool.h"
#include "../../inc/logger.h"

namespace collie {

EventThreadPool::EventThreadPool(unsigned thread_num) noexcept
    : thread_num_(thread_num) {}

EventThreadPool::~EventThreadPool() noexcept {
  try {
    Stop();
  } catch (...) {
  }
}

void EventThreadPool::Start() {
  std::lock_guard<std::mutex> lock(worker_mutex_);

  if (is_running_) {
    LOG(WARN) << "Event thread pool is running";
    return;
  }

  for (unsigned i = 0; i < thread_num_; ++i) {
    workers_.emplace_back(&EventThreadPool::EventLoop, this);
  }
  is_running_ = true;
}

void EventThreadPool::Stop() {
  std::lock_guard<std::mutex> lock(worker_mutex_);

  if (!is_running_) {
    LOG(WARN) << "Event thread pool is not running";
    return;
  }

  for (auto& worker : workers_) {
    worker.join();
  }
  is_running_ = false;
  workers_.clear();
}

void EventThreadPool::Push(IOStream io) noexcept {
  std::lock_guard<std::mutex> lock(worker_mutex_);
  if (!is_running_) {
    LOG(WARN) << "Event thread pool should start first";
    return;
  }

  ios_.push_back(io);
  io_condition_.notify_one();
}

void EventThreadPool::EventLoop() {
  auto event_pool = std::make_shared<EventPool>();

  event_pool->Init();

  while (is_running_) {
    std::vector<IOStream> ios;
    {
      std::unique_lock<std::mutex> lock(worker_mutex_);
      if (event_pool->IsEmpty() && ios_.empty()) io_condition_.wait(lock);
      ios.swap(ios_);
    }
    for (auto& io : ios) event_pool->Update(io);

    event_pool->LoopOne();
  }
  event_pool->Destroy();
}
}
