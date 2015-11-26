#include <vector>
#include "../../include/poll/epoll_poller.h"
#include "../../include/poll/poll_poller.h"
#include "../../include/event/eventloop.h"
#include "../../include/event/channel.h"
#include "../../include/descriptor.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace event {

EventLoop::EventLoop() : kPoller(new poll::EPollPoller(1024)) {
  Log(TRACE) << "EventLoop is constructing";
}

EventLoop::EventLoop(std::unique_ptr<poll::Poller> poller)
    : kPoller(std::move(poller)) {
  Log(TRACE) << "EventLoop is constructing";
}

EventLoop::~EventLoop() { Log(TRACE) << "EventLoop is destructing"; }

void EventLoop::LoopOne() {
  REQUIRE(kPoller->is_init());
  Log(TRACE) << "EventLoop is looping one";
  using namespace std::placeholders;
  kPoller->Poll(std::bind(&EventLoop::PollCallback, this, _1, _2));
}

void EventLoop::Loop() {
  REQUIRE(kPoller->is_init());
  while (true) {
    Log(TRACE) << "EventLoop is looping";
    using namespace std::placeholders;
    kPoller->Poll(std::bind(&EventLoop::PollCallback, this, _1, _2));
  }
}

void EventLoop::LoopNonBlocking() {
  REQUIRE(kPoller->is_init());
  Log(TRACE) << "EventLoop is looping";
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
    Log(WARN) << "Unknown channel " << fd;
  }
}

void EventLoop::UpdateChannel(std::shared_ptr<Channel> channel) {
  REQUIRE(kPoller->is_init() && channel);
  const auto descriptor = channel->descriptor()->fd();
  Log(TRACE) << "EventLoop update channel " << descriptor;
  if (HasChannel(channel)) {
    kPoller->Modify(descriptor, channel->events());
  } else if (channel->in_eventloop()) {
    THROW_("Channel " + std::to_string(descriptor) +
           "is not in this eventloop");
  } else {
    Log(TRACE) << "EventLoop insert channel " << descriptor;
    kPoller->Insert(descriptor, channel->events());
    channel_map_[descriptor] = channel;
    channel->in_eventloop_ = true;             // channel is in eventloop
    channel->eventloop_ = shared_from_this();  // link channel with event loop
    if (channel->insert_callback_)             // call channel inserted callback
      channel->insert_callback_(channel);
  }
}

void EventLoop::RemoveChannel(std::shared_ptr<Channel> channel) {
  REQUIRE(kPoller->is_init() && channel);
  const auto descriptor = channel->descriptor()->fd();
  Log(TRACE) << "EventLoop remove channel " << descriptor;
  REQUIRE_(HasChannel(channel),
           "EventLoop does NOT have channel " + std::to_string(descriptor));
  kPoller->Remove(descriptor);     // remove channel from poller
  channel_map_.erase(descriptor);  // remove from loop
  channel->in_eventloop_ = false;  // channel is not in eventloop
  if (channel_map_.empty()) {
    Log(WARN) << "Event loop is empty";
  }
}

bool EventLoop::HasChannel(std::shared_ptr<Channel> channel) const {
  REQUIRE(channel);
  const auto descriptor = channel->descriptor()->fd();
  return channel_map_.find(descriptor) != channel_map_.end();
}
}
}
