#include <collie/EventThreadPool.h>
#include <collie/Logger.h>

namespace collie {

EventThreadPool::EventThreadPool(const unsigned thread_num) noexcept
    : is_running(false),
      thread_num(thread_num) {}

EventThreadPool::~EventThreadPool() { Stop(); }

void EventThreadPool::Start() noexcept {
  if (is_running) {
    LOG(WARN) << "Event thread pool is running";
    return;
  }

  for (unsigned i = 0; i < thread_num; ++i) {
    workers.emplace_back(&EventThreadPool::EventLoop, this);
  }
  is_running = true;
  LOG(INFO) << "Enter Ctrl+C to quit...";
}

void EventThreadPool::Stop() noexcept {
  if (!is_running) {
    LOG(WARN) << "Event thread pool is not running";
    return;
  }

  is_running = false;
  for (auto& worker : workers) {
    worker.join();
  }
  workers.clear();
}

void EventThreadPool::Push(std::shared_ptr<AsyncIOStream> io,
                           bool to_all) noexcept {
  if (!is_running) {
    LOG(WARN) << "event thread pool is not running";
    return;
  }

  if (to_all) {
    // Inserts to all pools
    for (auto& e : event_pools) e.second->Update(io);
  } else {
    // All pools take turns at inserting io for balance
    static auto current_event_pool = event_pools.begin();
    if (current_event_pool == event_pools.end()) {
      current_event_pool = event_pools.begin();
    }
    current_event_pool->second->Update(io);
    ++current_event_pool;
  }
}

void EventThreadPool::EventLoop() {
  auto event_pool = std::make_shared<EventPool>();

  event_pools.insert({std::this_thread::get_id(), event_pool});

  event_pool->Loop();
}
}
