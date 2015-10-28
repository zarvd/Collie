#include "../../include/Global.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/EventLoop.hpp"
#include <unistd.h>


namespace Collie { namespace Event {

Channel::Channel(const int fd) :
    inEventLoop(false),
    fd(fd),
    events(0) {
    Log(TRACE) << "Channel " << fd << " constructing";

    // default callback
    closeCallback = [this]() {
        // TODO remove channel
        Log(TRACE) << "Close channel " << this->getFd();
    };
    errorCallback = [this]() {
        // TODO remove channel
        Log(TRACE) << "Channel " << this->getFd() << " meets ERROR";
    };
}

Channel::~Channel() {
    Log(TRACE) << "Channel " << fd << " destructing";
    if(::close(fd) == -1) {
        Log(WARN) << "Channel " << fd << " is already closed";
    }
}

void
Channel::setEventLoop(std::shared_ptr<EventLoop> eventLoop) {
    if(inEventLoop) {
        Log(WARN) << "Channel" << fd << " is already in eventLoop";
    } else {
        this->eventLoop = eventLoop;
        inEventLoop = true;
    }
}

bool
Channel::isRead() const {
    return eventLoop->isEventRead(events);
}

bool
Channel::isWrite() const {
    return eventLoop->isEventWrite(events);
}

void
Channel::enableRead() {
    eventLoop->enableEventRead(events);
    update();
}

void
Channel::disableRead() {
    eventLoop->disableEventRead(events);
    update();
}

void
Channel::enableWrite() {
    eventLoop->enableEventWrite(events);
    update();
}

void
Channel::disableWrite() {
    eventLoop->disableEventWrite(events);
    update();
}

void
Channel::disableAll() {
    // FIXME
    events = 0;
    update();
}

void
Channel::activate(const unsigned revents) const {
    if(eventLoop->isEventError(revents)) {
        Log(TRACE) << "Activate ERROR callback with events " << revents;
        if(errorCallback) {
            errorCallback();
        } else {
            Log(ERROR) << "errorCallback is not callable";
            THROW_NOTFOUND;
        }
    } else if(eventLoop->isEventClose(revents)) {
        Log(TRACE) << "Activate CLOSE callback with events" << revents;
        if(closeCallback) {
            closeCallback();
        } else {
            Log(ERROR) << "closeCallback is not callable";
            THROW_NOTFOUND;
        }
    } else if(eventLoop->isEventRead(revents)) {
        if(isRead()) {
            Log(TRACE) << "Activate READ callback with events " << revents;
            if(readCallback) {
                readCallback();
            } else {
                Log(ERROR) << "readCallback is not callable";
                THROW_NOTFOUND;
            }
        } else {
            Log(WARN) << "READ callback is not available";
        }
    } else if(eventLoop->isEventWrite(revents)) {
        if(isWrite()) {
            Log(TRACE) << "Activate WRITE callback with events" << revents;
            if(writeCallback) {
                writeCallback();
            } else {
                Log(ERROR) << "writeCallback is not callable";
                THROW_NOTFOUND;
            }
        } else {
            Log(WARN) << "WRITE callback is not available";
        }
    }
}

void
Channel::update() {
    eventLoop->updateChannel(shared_from_this());
}

void
Channel::remove() {
    eventLoop->removeChannel(shared_from_this());
}

}}
