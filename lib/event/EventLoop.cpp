#include <vector>
#include "../../include/Global.hpp"
#include "../../include/poll/EPollPoller.hpp"
#include "../../include/poll/PollPoller.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/Descriptor.hpp"

namespace Collie {
namespace Event {

EventLoop::EventLoop() : poller(new Poll::EPollPoller(1024)) {
    poller->create();

    Log(TRACE) << "EventLoop is constructing";
}

EventLoop::EventLoop(std::unique_ptr<Poll::Poller> poller)
    : poller(std::move(poller)) {
    poller->create();

    Log(TRACE) << "EventLoop is constructing";
}

EventLoop::~EventLoop() { Log(TRACE) << "EventLoop is destructing"; }

void
EventLoop::loopOne() {
    Log(TRACE) << "EventLoop is looping one";
    using namespace std::placeholders;
    poller->poll(std::bind(&EventLoop::pollCallback, this, _1, _2));
}

void
EventLoop::loop() {
    while(true) {
        Log(TRACE) << "EventLoop is looping";
        using namespace std::placeholders;
        poller->poll(std::bind(&EventLoop::pollCallback, this, _1, _2));
    }
}

void
EventLoop::loopNonBlocking() {
    Log(TRACE) << "EventLoop is looping";
    using namespace std::placeholders;
    poller->poll(std::bind(&EventLoop::pollCallback, this, _1, _2),
                 0); // should be non blocking
}

void
EventLoop::pollCallback(const int fd, const unsigned revents) {
    // find channel
    auto it = this->channels.find(fd);
    if(it != this->channels.end()) {
        auto channel = it->second;
        channel->activate(revents);
    } else {
        Log(WARN) << "Unknown channel " << fd;
    }
}

void
EventLoop::updateChannel(std::shared_ptr<Channel> channel) {
    const auto descriptor = channel->getDescriptor()->get();
    Log(TRACE) << "EventLoop update channel " << descriptor;
    if(hasChannel(channel)) {
        poller->modify(descriptor, channel->getEvents());
    } else {
        poller->insert(descriptor, channel->getEvents());
        channels[descriptor] = channel;
    }
}

void
EventLoop::removeChannel(std::shared_ptr<Channel> channel) {
    const auto descriptor = channel->getDescriptor()->get();
    Log(TRACE) << "EventLoop remove channel " << descriptor;
    REQUIRE_(hasChannel(channel),
             "EventLoop does NOT have channel " + std::to_string(descriptor));
    poller->remove(descriptor); // remove channel from poller
    channels.erase(descriptor); // remove from loop
}

bool
EventLoop::hasChannel(std::shared_ptr<Channel> channel) const {
    const auto descriptor = channel->getDescriptor()->get();
    return channels.find(descriptor) != channels.end();
}
}
}
