#include "Channel.hpp"
#include "EventLoop.hpp"


namespace MiniHttp { namespace Base {

Channel::Channel(std::shared_ptr<EventLoop> eventLoop, const int& fd) :
    fd(fd),
    events(0),
    eventLoop(eventLoop),
    inLoop(true) {
    // TODO set default close and error callback
}

Channel::~Channel() {}

void Channel::activate(const unsigned & revents) const {
    if(Event::isError(revents)) {
        errorCallback();
    } else if(Event::isClose(revents)) {
        closeCallback();
    } else if(Event::isRead(revents)) {
        if(isRead()) {
            readCallback();
        }
    } else if(Event::isWrite(revents)) {
        if(isWrite()) {
            writeCallback();
        }
    }
}

}}
