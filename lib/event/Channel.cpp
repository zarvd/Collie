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
}

void
Channel::disableRead() {
    eventLoop->disableEventRead(events);
}

void
Channel::enableWrite() {
    eventLoop->enableEventWrite(events);
}

void
Channel::disableWrite() {
    eventLoop->disableEventWrite(events);
}

void
Channel::disableAll() {
    // FIXME
    events = 0;
}

void
Channel::activate(const unsigned & revents) const {
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
Channel::update() const {
    eventLoop->updateChannel(fd);
}

void
Channel::remove() const {
    close(fd);  // FIXME
    eventLoop->removeChannel(fd);
}

}}
