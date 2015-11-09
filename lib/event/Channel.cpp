#include "../../include/Global.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/poll/EPollPoller.hpp"
#include <unistd.h>

namespace Collie {
namespace Event {

Channel::Channel(const int fd)
    : isOwnFd(true),
      inEventLoop(false),
      fd(fd),
      events(0),
      updateAfterActivate(false) {
    Log(TRACE) << "Channel " << fd << " constructing";

    // default error and close callback
    closeCallback = [this]() {
        // TODO remove channel
        Log(TRACE) << "Close channel " << this->getFd();
        this->remove();
    };
    errorCallback = [this]() {
        // TODO remove channel
        Log(TRACE) << "Channel " << this->getFd() << " meets ERROR";
        this->remove();
    };
}

Channel::~Channel() {
    Log(TRACE) << "Channel " << fd << " destructing";
    if(isOwnFd && ::close(fd) == -1) {
        // close socket
        Log(WARN) << "Channel " << fd << " is already closed";
        // don't throw
    }
}

std::shared_ptr<Channel>
Channel::getCopyWithoutEventLoop() const {
    auto channel = std::make_shared<Channel>(fd);
    channel->isOwnFd = isOwnFd;
    channel->events = events;
    channel->readCallback = readCallback;
    channel->writeCallback = writeCallback;
    channel->errorCallback = errorCallback;
    channel->closeCallback = closeCallback;
    channel->afterSetLoopCallback = afterSetLoopCallback;
    return channel;
}

void
Channel::setEventLoop(std::shared_ptr<EventLoop> eventLoop) {
    if(inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel" + std::to_string(fd) +
                                " is already in eventLoop");
    } else {
        this->eventLoop = eventLoop;
        inEventLoop = true;
        if(afterSetLoopCallback) afterSetLoopCallback(shared_from_this());
    }
}

bool
Channel::isRead() const {
    if(!inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel is not in event loop");
    }
    return eventLoop->poller->isRead(events);
}

bool
Channel::isWrite() const {
    if(!inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel is not in event loop");
    }
    return eventLoop->poller->isWrite(events);
}

void
Channel::enableRead() {
    if(!inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel is not in event loop");
    }
    eventLoop->poller->enableRead(events);
    update();
}

void
Channel::disableRead() {
    if(!inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel is not in event loop");
    }

    eventLoop->poller->disableRead(events);
    update();
}

void
Channel::enableWrite() {
    if(!inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel is not in event loop");
    }

    eventLoop->poller->enableWrite(events);
    update();
}

void
Channel::disableWrite() {
    if(!inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel is not in event loop");
    }

    eventLoop->poller->disableWrite(events);
    update();
}

void
Channel::enableOneShot() {
    if(!inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel is not in event loop");
    }

    eventLoop->poller->enableOneShot(events);
    update();
}

void
Channel::disableOneShot() {
    if(!inEventLoop) {
        THROW_INVALID_ARGUMENT_("Channel is not in event loop");
    }

    eventLoop->poller->disableOneShot(events);
    update();
}

void
Channel::disableAll() {
    // FIXME
    events = 0;
    update();
}

void
Channel::activate(const unsigned revents) {
    if(!eventLoop->poller) {
        THROW_INVALID_ARGUMENT_("Poller is null");
    }
    if(eventLoop->poller->isError(revents)) {
        // error event
        Log(TRACE) << "Activate ERROR callback with events " << revents;
        if(errorCallback) {
            errorCallback();
        } else {
            THROW_NOTFOUND_("errorCallback is not callable");
        }
    } else if(eventLoop->poller->isClose(revents)) {
        // close event
        Log(TRACE) << "Activate CLOSE callback with events" << revents;
        if(closeCallback) {
            closeCallback();
        } else {
            THROW_NOTFOUND_("closeCallback is not callable");
        }
    } else {
        // read event
        if(eventLoop->poller->isRead(revents)) {
            if(isRead()) {
                Log(TRACE) << "Activate READ callback with events " << revents;
                if(readCallback) {
                    readCallback();
                } else {
                    THROW_NOTFOUND_("readCallback is not callable");
                }
            } else {
                Log(WARN) << "READ callback is not available";
            }
        }
        // write event
        if(eventLoop->poller->isWrite(revents)) {
            if(isWrite()) {
                Log(TRACE) << "Activate WRITE callback with events" << revents;
                if(writeCallback) {
                    writeCallback();
                } else {
                    THROW_NOTFOUND_("writeCallback is not callable");
                }
            } else {
                Log(WARN) << "WRITE callback is not available";
            }
        }
    }
    if(updateAfterActivate) update();
}

void
Channel::update() {
    eventLoop->updateChannel(shared_from_this());
}

void
Channel::remove() {
    eventLoop->removeChannel(shared_from_this());
}
}
}
