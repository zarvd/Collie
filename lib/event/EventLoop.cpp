#include <vector>
#include "../../include/Global.hpp"
#include "../../include/poll/EPollPoller.hpp"
#include "../../include/poll/PollPoller.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"


namespace Collie { namespace Event {

EventLoop::EventLoop() noexcept :
    poller(new Poll::EPollPoller(1024)),
    channels(new ChannelMap) {

    Log(TRACE) << "EventLoop constructing";
}

EventLoop::~EventLoop() {
    Log(TRACE) << "EventLoop destructing";
}

void
EventLoop::loop() {
    while(true) {
        Log(TRACE) << "EventLoop looping";
        using namespace std::placeholders;
        poller->poll(std::bind(&EventLoop::pollCallback, this, _1, _2));
   }
}

void
EventLoop::loopNonBlocking() {
    Log(TRACE) << "EventLoop is looping";
    using namespace std::placeholders;
    poller->poll(std::bind(&EventLoop::pollCallback, this, _1, _2), 0);  // should be non blocking
}

void
EventLoop::pollCallback(const unsigned fd, const unsigned revents) {
    // find channel
    // activate channel according to event type
    auto it = this->channels->find(fd);
    if(it != this->channels->end()) {
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
        (*channels)[channel->getFd()] = channel;
    }
}

void
EventLoop::removeChannel(std::shared_ptr<Channel> channel) {
    Log(TRACE) << "EventLoop remove channel " << channel->getFd();
    if(hasChannel(channel)) {
        poller->remove(channel->getFd());
        channels->erase(channel->getFd());
    } else {
        Log(WARN) << "EventLoop does NOT have channel " << channel->getFd();
        THROW_NOTFOUND;
    }
}

bool
EventLoop::hasChannel(std::shared_ptr<Channel> channel) const {
    return channels->find(channel->getFd()) != channels->end();
}

void
EventLoop::enableEventRead(unsigned & events) const noexcept {
    poller->enableRead(events);
}

void
EventLoop::disableEventRead(unsigned & events) const noexcept {
    poller->disableRead(events);
}

void
EventLoop::enableEventWrite(unsigned & events) const noexcept {
    poller->enableWrite(events);
}

void
EventLoop::disableEventWrite(unsigned & events) const noexcept {
    poller->disableWrite(events);
}

bool
EventLoop::isEventRead(const unsigned events) const noexcept {
    return poller->isRead(events);
}

bool
EventLoop::isEventWrite(const unsigned events) const noexcept {
    return poller->isWrite(events);
}

bool
EventLoop::isEventError(const unsigned events) const noexcept {
    return poller->isError(events);
}

bool
EventLoop::isEventClose(const unsigned events) const noexcept {
    return poller->isClose(events);
}

}}
