#include "../../inc/base/EventThreadPool.h"
#include <signal.h>
#include <cstdio>
#include "../../inc/base/AsyncIOStream.h"
#include "../../inc/base/EventPool.h"
#include "../../inc/base/Logger.h"

namespace collie {

sig_atomic_t SIGNAL_STOP_FLAG = true;

thread_local std::shared_ptr<EventPool> EventThreadPool::current_event_pool;

EventThreadPool::EventThreadPool(unsigned thread_num) noexcept
    : thread_num(thread_num) {}

EventThreadPool::~EventThreadPool() noexcept {
  LOG(DEBUG) << "event thread pool is stopping";
  Stop();
}

void EventThreadPool::Start(bool is_forever) noexcept {
  if (is_running) {
    LOG(WARN) << "Event thread pool is running";
    return;
  }

  for (unsigned i = 0; i < thread_num; ++i) {
    workers.emplace_back(&EventThreadPool::EventLoop, this);
  }
  is_running = true;
  if (is_forever) {
    LOG(INFO) << "Enter Ctrl+C to quit...";
    signal(SIGINT, [](int) -> void { SIGNAL_STOP_FLAG = false; });
    while (SIGNAL_STOP_FLAG) {
    }
  }
}

void EventThreadPool::Stop() noexcept {
  if (!is_running) {
    LOG(DEBUG) << "Event thread pool is not running";
    return;
  }

  is_running = false;
  for (auto& worker : workers) {
    worker.join();
  }
  workers.clear();
}

void EventThreadPool::PushInit(IOStream io) noexcept {
  init_io_streams.push_back(io);
}

void EventThreadPool::Push(IOStream io) noexcept {
  if (!is_running) {
    LOG(DEBUG) << "event thread pool is not running";
    return;
  }
  current_event_pool->Update(io);
}

void EventThreadPool::EventLoop() {
  current_event_pool = std::make_shared<EventPool>();

  current_event_pool->Init();
  LOG(DEBUG) << "event pool started";

  for (auto& io : init_io_streams) {
    current_event_pool->Update(io);
  }

  LOG(DEBUG) << "event pool looping";
  current_event_pool->Loop();

  LOG(DEBUG) << "event pool stopped";
  current_event_pool->Destroy();
}
}
