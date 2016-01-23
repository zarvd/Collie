#include "../../include/event/eventloop_threadpool.h"
#include "../../include/event/channel.h"
#include "../../include/event/eventloop.h"
#include "../../include/logging.h"

namespace collie {
namespace event {

EventLoopThreadPool::EventLoopThreadPool(const size_t thread_num)
    : kThreadNum(thread_num), terminate_(false) {}

EventLoopThreadPool::~EventLoopThreadPool() { terminate_ = true; }

void EventLoopThreadPool::StartLoop(
    std::vector<std::shared_ptr<Channel>> base_channels) {
  base_channels_ = base_channels;
  for (size_t i = 1; i < kThreadNum; ++i) {
    workers_.emplace_back(&EventLoopThreadPool::RunInThread, this);
  }
  RunInThread();
}

std::shared_ptr<EventLoop> EventLoopThreadPool::GetCurrentEventLoop() {
  auto global_pool = GetEventLoopThreadPool();
  return global_pool->eventloops_.at(std::this_thread::get_id());
}

std::shared_ptr<EventLoopThreadPool::ChannelQueue>
EventLoopThreadPool::GetNextQueue() {
  std::lock_guard<std::mutex> lock(eventloop_thread_mtx_);
  static auto it = channel_queues_.begin();
  if (it == channel_queues_.end()) {
    it = channel_queues_.begin();
  }
  return *(it++);
}

void EventLoopThreadPool::PushChannel(std::shared_ptr<Channel> channel) {
  // insert to NEXT loop
  auto global_pool = GetEventLoopThreadPool();
  auto queue = global_pool->GetNextQueue();
  std::lock_guard<std::mutex> lock(queue->mtx);
  queue->channels.push_back(channel);
}

void EventLoopThreadPool::RunInThread() {
  auto channel_queue = std::make_shared<ChannelQueue>();
  // one loop per thread
  auto eventloop = std::make_shared<EventLoop>();
  {
    std::lock_guard<std::mutex> lock(eventloop_thread_mtx_);
    channel_queues_.push_back(channel_queue);
    eventloops_[std::this_thread::get_id()] = eventloop;
  }

  // insert base channels to loop
  for (auto channel : base_channels_) {
    auto new_channel = channel->GetCopyWithoutEventLoop();
    eventloop->UpdateChannel(new_channel);
  }

  // loop
  while (true) {
    TIMED_SCOPE(timerBlkObj, "event loop thread pool");
    std::vector<std::shared_ptr<Channel>> channels;
    // get new channel or terminate
    // Non blocking
    if (channel_queue->mtx.try_lock()) {
      if (terminate_ || !channel_queue->channels.empty()) {
        if (terminate_ && channel_queue->channels.empty()) return;  // exit
        std::swap(channel_queue->channels, channels);
      }
      channel_queue->mtx.unlock();
    }
    // insert channel
    for (auto channel : channels) {
      eventloop->UpdateChannel(channel);
    }
    // block
    eventloop->LoopOne();
  }
}
}
}
