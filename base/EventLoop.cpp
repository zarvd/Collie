#include <algorithm>
#include "EventLoop.hpp"


EventLoop::EventLoop() : poller(1024), isLooping(false) {}

EventLoop::~EventLoop() {}

void EventLoop::loop() {}

void EventLoop::updateChannel(std::shared_ptr<Channel> channel) {
    if(hasChannel(channel)) {
        poller.modify(channel->getFd(), channel->getEvents());
    } else {
        channelList.insert(channel);
    }
}

void EventLoop::removeChannel(std::shared_ptr<Channel> channel) {
    if(hasChannel(channel)) {
        poller.remove(channel->getFd());
        channelList.erase(channel);
    }
}

bool EventLoop::hasChannel(std::shared_ptr<Channel> channel) const {
    return channelList.find(channel) != channelList.end();
}
