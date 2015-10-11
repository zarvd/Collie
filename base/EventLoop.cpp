#include <vector>
#include "EventLoop.hpp"
#include "EPoller.hpp"
#include "Channel.hpp"

namespace MiniHttp { namespace Base {

EventLoop::EventLoop() :
    poller(new EPoller(1024)),
    channels(new ChannelMap),
    isLooping(false) {}

EventLoop::~EventLoop() {}

void EventLoop::loop() {
    isLooping = true;
    while(true) {
        poller->poll(channels);
    }
}

void EventLoop::updateChannel(std::shared_ptr<Channel> channel) {
    if(hasChannel(channel)) {
        poller->modify(channel->getFd(), channel->getEvents());
    } else {
        poller->insert(channel->getFd(), channel->getEvents());
    }
}

void EventLoop::removeChannel(std::shared_ptr<Channel> channel) {
    if(hasChannel(channel)) {
        poller->remove(channel->getFd());
        channels->erase(channel->getFd());
    }
}

bool EventLoop::hasChannel(std::shared_ptr<Channel> channel) const {
    return channels->find(channel->getFd()) != channels->end();
}

}}
