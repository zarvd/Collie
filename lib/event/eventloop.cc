#include <vector>
#include "../../include/event/epoller.h"
#include "../../include/event/eventloop.h"
#include "../../include/event/channel.h"
#include "../../include/descriptor.h"
#include "../../include/logging.h"

namespace collie {
namespace event {

EventLoop::EventLoop() : kPoller(new EPoller(1024)) {}

EventLoop::EventLoop(std::unique_ptr<EPoller> poller)
    : kPoller(std::move(poller)) {}

EventLoop::~EventLoop() {}

void EventLoop::LoopOne(const unsigned timeout) {
  CHECK(kPoller->is_init());
  using namespace std::placeholders;
  kPoller->Poll(std::bind(&EventLoop::PollCallback, this, _1, _2));
}

void EventLoop::Loop(const unsigned timeout) {
  CHECK(kPoller->is_init());
  while (true) {
    using namespace std::placeholders;
    kPoller->Poll(std::bind(&EventLoop::PollCallback, this, _1, _2));
  }
}

void EventLoop::LoopNonBlocking() {
  CHECK(kPoller->is_init());
  using namespace std::placeholders;
  kPoller->Poll(std::bind(&EventLoop::PollCallback, this, _1, _2),
                0);  // should be non blocking
}

void EventLoop::PollCallback(const int fd, const unsigned revents) {
  // find channel
  auto it = this->channel_map_.find(fd);
  if (it != this->channel_map_.end()) {
    auto channel = it->second;
    channel->Activate(revents);
  } else {
    LOG(WARNING) << "Unknown channel " << fd;
  }
}

void EventLoop::UpdateChannel(std::shared_ptr<Channel> channel) {
  CHECK(kPoller->is_init() && channel);
  const auto descriptor = channel->descriptor()->fd();
  if (HasChannel(channel)) {
    kPoller->Modify(descriptor, channel->events());
  } else {
    CHECK(!channel->in_eventloop()) << "Channel" << descriptor
                                    << "is not in this eventloop";
    kPoller->Insert(descriptor, channel->events());
    channel_map_[descriptor] = channel;
    channel->in_eventloop_ = true;             // channel is in eventloop
    channel->eventloop_ = shared_from_this();  // link channel with event loop
    if (channel->insert_callback_)             // call channel inserted callback
      channel->insert_callback_(channel);
  }
}

void EventLoop::RemoveChannel(std::shared_ptr<Channel> channel) {
  CHECK(kPoller->is_init() && channel);
  const auto descriptor = channel->descriptor()->fd();
  CHECK(HasChannel(channel)) << "EventLoop does NOT have channel" << descriptor;
  kPoller->Remove(descriptor);     // remove channel from poller
  channel_map_.erase(descriptor);  // remove from loop
  channel->in_eventloop_ = false;  // channel is not in eventloop
  if (channel_map_.empty()) {
    LOG(WARNING) << "Event loop is empty";
  }
}

bool EventLoop::HasChannel(std::shared_ptr<Channel> channel) const {
  CHECK(channel);
  const auto descriptor = channel->descriptor()->fd();
  return channel_map_.find(descriptor) != channel_map_.end();
}
}
}
