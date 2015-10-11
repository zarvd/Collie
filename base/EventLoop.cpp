#include <vector>
#include "EventLoop.hpp"
#include "EPoller.hpp"
#include "Channel.hpp"
#include "../Httpd.hpp"


namespace MiniHttp { namespace Base {

EventLoop::EventLoop() :
    poller(new EPoller(1024)),
    channels(new ChannelMap),
    isLooping(false) {

    Log(DEBUG) << "EventLoop constructing";
}

EventLoop::~EventLoop() {
    Log(DEBUG) << "EventLoop destructing";
}

void EventLoop::loop() {
    isLooping = true;
    while(true) {
        Log(DEBUG) << "EventLoop looping";
        poller->poll(channels);
    }
}

void EventLoop::updateChannel(std::shared_ptr<Channel> channel) {
    Log(DEBUG) << "EventLoop update channel " << channel->getFd();
    if(hasChannel(channel)) {
        poller->modify(channel->getFd(), channel->getEvents());
    } else {
        if(poller->insert(channel->getFd(), channel->getEvents()) == Status::SUCCESS) {
            (*channels)[channel->getFd()] = channel;
        }
    }
}

void EventLoop::removeChannel(std::shared_ptr<Channel> channel) {
    Log(DEBUG) << "EventLoop remove channel " << channel->getFd();
    if(hasChannel(channel)) {
        poller->remove(channel->getFd());
        channels->erase(channel->getFd());
    } else {
        Log(WARN) << "EventLoop does NOT have channel " << channel->getFd();
    }
}

bool EventLoop::hasChannel(std::shared_ptr<Channel> channel) const {
    return channels->find(channel->getFd()) != channels->end();
}

}}
