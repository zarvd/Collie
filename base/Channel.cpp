#include "Channel.hpp"
#include "EventLoop.hpp"


Channel::Channel(std::shared_ptr<EventLoop> eventLoop, const int& fd) :
    fd(fd),
    events(0),
    eventLoop(eventLoop),
    inLoop(true) {
    // set default close and error callback
}

Channel::~Channel() {}

void Channel::update() {
    if(inLoop) {
        eventLoop->updateChannel(std::shared_ptr<Channel>(this));
    }
}

void Channel::remove() {
    if( ! inLoop) {
        eventLoop->removeChannel(std::shared_ptr<Channel>(this));
    }
}
