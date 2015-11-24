#include "../../include/event/eventloop_threadpool.h"
#include "../../include/event/channel.h"
#include "../../include/event/eventloop.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace event {

EventLoopThreadPool::EventLoopThreadPool(const size_t thread_num)
    : kThreadNum(thread_num) {}

EventLoopThreadPool::~EventLoopThreadPool() {}

void EventLoopThreadPool::StartLoop(
    std::vector<std::shared_ptr<Channel>> baseChannel) {
  base_channel_ = baseChannel;
  for (size_t i = 1; i < kThreadNum; ++i) {
    workers_.emplace_back(&EventLoopThreadPool::RunInThread, this);
  }
  RunInThread();
}

std::shared_ptr<EventLoopThreadPool::EventLoopThread>
EventLoopThreadPool::GetNextLoop() {
  std::lock_guard<std::mutex> lock(eventloop_thread_mtx_);
  static auto it = eventloop_threads_.begin();
  if (it == eventloop_threads_.end()) {
    it = eventloop_threads_.begin();
  }
  return *(it++);
}

void EventLoopThreadPool::PushChannel(std::shared_ptr<Channel> channel) {
  // insert to NEXT loop
  Log(TRACE) << "pushing channel";
  auto loop = GetNextLoop();
  std::lock_guard<std::mutex> lock(loop->mtx);
  loop->channels.push_back(channel);
}

void EventLoopThreadPool::RunInThread() {
  auto eventloop_thread = std::make_shared<EventLoopThread>();
  {
    std::lock_guard<std::mutex> lock(eventloop_thread_mtx_);
    eventloop_threads_.push_back(eventloop_thread);
  }

  // one loop per thread
  auto eventloop = std::make_shared<EventLoop>();

  // insert base channels to loop
  for (auto channel : base_channel_) {
    auto new_channel = channel->GetCopyWithoutEventLoop();
    new_channel->set_eventloop(eventloop);
    new_channel->Update();
  }

  // loop
  while (true) {
    std::vector<std::shared_ptr<Channel>> channels;
    // get new channel or terminate
    // Non blocking
    if (eventloop_thread->mtx.try_lock()) {
      if (terminate_ || !eventloop_thread->channels.empty()) {
        if (terminate_ && eventloop_thread->channels.empty()) return;  // exit
        std::swap(eventloop_thread->channels, channels);
      }
      eventloop_thread->mtx.unlock();
    }
    // insert channel
    for (auto channel : channels) {
      channel->set_eventloop(eventloop);
      channel->Update();
    }
    // block
    eventloop->LoopOne();
  }
}
}
}
