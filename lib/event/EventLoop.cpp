#include <vector>
#include "../../include/Global.hpp"
#include "../../include/poll/EPollPoller.hpp"
#include "../../include/poll/PollPoller.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"

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
    Log(TRACE) << "EventLoop update channel " << channel->getFd();
    if(hasChannel(channel)) {
        poller->modify(channel->getFd(), channel->getEvents());
    } else {
        poller->insert(channel->getFd(), channel->getEvents());
        channels[channel->getFd()] = channel;
    }
}

void
EventLoop::removeChannel(std::shared_ptr<Channel> channel) {
    Log(TRACE) << "EventLoop remove channel " << channel->getFd();
    if(hasChannel(channel)) {
        poller->remove(channel->getFd()); // remove channel from poller
        channels.erase(channel->getFd()); // remove from loop
    } else {
        THROW_("EventLoop does NOT have channel " +
                        std::to_string(channel->getFd()));
    }
}

bool
EventLoop::hasChannel(std::shared_ptr<Channel> channel) const {
    return channels.find(channel->getFd()) != channels.end();
}
}
}
