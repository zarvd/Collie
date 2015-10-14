#include <vector>
#include "../Global.hpp"
#include "../poll/EPollPoller.hpp"
#include "../poll/PollPoller.hpp"
#include "EventLoop.hpp"
#include "Channel.hpp"


namespace Collie { namespace Base { namespace Event {

EventLoop::EventLoop() :
    poller(new Poll::EPollPoller(1024)),
    channels(new ChannelMap),
    isLooping(false) {

    Log(TRACE) << "EventLoop constructing";
}

EventLoop::~EventLoop() {
    Log(TRACE) << "EventLoop destructing";
}

void EventLoop::loop() {
    isLooping = true;
    while(true) {
        Log(TRACE) << "EventLoop looping";
        // poller->poll(channels);
    }
}

void EventLoop::updateChannel(std::shared_ptr<Channel> channel) {
    Log(TRACE) << "EventLoop update channel " << channel->getFd();
    if(hasChannel(channel)) {
        poller->modify(channel->getFd(), channel->getEvents());
    } else {
        poller->insert(channel->getFd(), channel->getEvents());
        (*channels)[channel->getFd()] = channel;
        channel->goInEventLoop();
    }
}

void EventLoop::updateChannel(const int fd) {
    Log(TRACE) << "EventLoop update channel(fd) " << fd;
    if(channels->find(fd) != channels->end()) {
        // FIXME
        auto channel = channels->at(fd);
        poller->modify(channel->getFd(), channel->getEvents());
    } else {
        Log(WARN) << "Channel " << fd << " is not in EventLoop";
    }
}

void EventLoop::removeChannel(std::shared_ptr<Channel> channel) {
    Log(TRACE) << "EventLoop remove channel " << channel->getFd();
    if(hasChannel(channel)) {
        poller->remove(channel->getFd());
        channels->erase(channel->getFd());
        channel->goOutEventLoop();
    } else {
        Log(WARN) << "EventLoop does NOT have channel " << channel->getFd();
    }
}

void EventLoop::removeChannel(const int fd) {
    if(channels->find(fd) != channels->end()) {
        // FIXME
        auto channel = channels->at(fd);
        poller->remove(channel->getFd());
        channels->erase(fd);
        channel->goOutEventLoop();
    } else {
        Log(WARN) << "Channel " << fd << " is not in EventLoop";
    }
}

bool EventLoop::hasChannel(std::shared_ptr<Channel> channel) const {
    return channels->find(channel->getFd()) != channels->end();
}

}}}
