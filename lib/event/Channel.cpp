#include "../../include/Global.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/EventLoop.hpp"


namespace Collie { namespace Event {

Channel::Channel(std::shared_ptr<EventLoop> eventLoop, const int& fd) :
    inEventLoop(false),
    fd(fd),
    events(0),
    eventLoop(eventLoop) {
    Log(TRACE) << "Channel " << fd << " constructing";

    // default callback
    closeCallback = [](const int fd) {
        // TODO remove channel
        Log(TRACE) << "Close channel " << fd;
    };
    errorCallback = [](const int fd) {
        // TODO remove channel
        Log(TRACE) << "Channel " << fd << " meets ERROR";
    };
}

Channel::~Channel() {
    Log(TRACE) << "Channel " << fd << " destructing";
}

void
Channel::goInEventLoop() {
    if( ! inEventLoop) {
        inEventLoop = true;
    } else {
        Log(WARN) << "Channel " << fd << " is already in a EventLoop";
    }
}

void
Channel::goOutEventLoop() {
    if(inEventLoop) {
        inEventLoop = false;
    } else {
        Log(WARN) << "Channel " << fd << " is not in a EventLoop";
    }
}

void
Channel::activate(const unsigned & revents) const {
    if(eventLoop->isEventError(revents)) {
        Log(TRACE) << "Activate ERROR callback with events " << revents;
        errorCallback(fd);
    } else if(eventLoop->isEventClose(revents)) {
        Log(TRACE) << "Activate CLOSE callback with events" << revents;
        closeCallback(fd);
    } else if(eventLoop->isEventRead(revents)) {
        if(isRead()) {
            Log(TRACE) << "Activate READ callback with events " << revents;
            readCallback(fd);
        } else {
            Log(WARN) << "READ callback is not available";
        }
    } else if(eventLoop->isEventWrite(revents)) {
        if(isWrite()) {
            Log(TRACE) << "Activate WRITE callback with events" << revents;
            writeCallback(fd);
        } else {
            Log(WARN) << "WRITE callback is not available";
        }
    }
}

void
Channel::update() const {
    eventLoop->updateChannel(fd);
}

void
Channel::remove() const {
    close(fd);  // FIXME
    eventLoop->removeChannel(fd);
}

}}
