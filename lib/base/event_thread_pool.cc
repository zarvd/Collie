#include "../../inc/poll/async_io_stream.h"
#include "../../inc/poll/event_pool.h"
#include "../../inc/poll/event_thread_pool.h"
#include "../../inc/logger.h"

namespace collie {

thread_local std::shared_ptr<EventPool> EventThreadPool::CurrentEventPool;

EventThreadPool::EventThreadPool(unsigned thread_num) noexcept
    : thread_num_(thread_num) {}

EventThreadPool::~EventThreadPool() noexcept {
  try {
    LOG(WARN) << "event thread pool is stopping";
    Stop();
  } catch (...) {
  }
}

void EventThreadPool::Start() {
  if (is_running_) {
    LOG(WARN) << "Event thread pool is running";
    return;
  }

  for (unsigned i = 1; i < thread_num_; ++i) {
    workers_.emplace_back(&EventThreadPool::EventLoop, this);
  }
  is_running_ = true;

  EventLoop();
}

void EventThreadPool::Stop() {
  if (!is_running_) {
    LOG(WARN) << "Event thread pool is not running";
    return;
  }

  is_running_ = false;
  for (auto& worker : workers_) {
    worker.join();
  }
  workers_.clear();
}

void EventThreadPool::PushInit(IOStream io) noexcept {
  init_ios_.push_back(io);
}

void EventThreadPool::Push(IOStream io) noexcept {
  CurrentEventPool->Update(io);
}

void EventThreadPool::EventLoop() {
  CurrentEventPool = std::make_shared<EventPool>();

  CurrentEventPool->Init();
  LOG(INFO) << "event pool started";

  for (auto& io : init_ios_) {
    CurrentEventPool->Update(io);
  }

  LOG(INFO) << "event pool looping";
  CurrentEventPool->Loop();

  LOG(INFO) << "event pool stopped";
  CurrentEventPool->Destroy();
}
}
