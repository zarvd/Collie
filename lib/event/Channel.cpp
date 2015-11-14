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
    REQUIRE_(!inEventLoop,
             "Channel" + std::to_string(fd) + " is already in eventLoop");
    this->eventLoop = eventLoop;
    inEventLoop = true;
    if(afterSetLoopCallback) afterSetLoopCallback(shared_from_this());
}

bool
Channel::isRead() const {
    REQUIRE_(inEventLoop, "Channel is not in event loop");
    TRACE_LOG;
    return eventLoop->poller->isRead(events);
}

bool
Channel::isWrite() const {
    REQUIRE_(inEventLoop, "Channel is not in event loop");
    TRACE_LOG;
    return eventLoop->poller->isWrite(events);
}

void
Channel::enableRead() {
    REQUIRE_(inEventLoop, "Channel is not in event loop");
    TRACE_LOG;
    eventLoop->poller->enableRead(events);
    update();
}

void
Channel::disableRead() {
    REQUIRE_(inEventLoop, "Channel is not in event loop");
    TRACE_LOG;
    eventLoop->poller->disableRead(events);
    update();
}

void
Channel::enableWrite() {
    REQUIRE_(inEventLoop, "Channel is not in event loop");
    TRACE_LOG;
    eventLoop->poller->enableWrite(events);
    update();
}

void
Channel::disableWrite() {
    REQUIRE_(inEventLoop, "Channel is not in event loop");
    TRACE_LOG;
    eventLoop->poller->disableWrite(events);
    update();
}

void
Channel::enableOneShot() {
    REQUIRE_(inEventLoop, "Channel is not in event loop");
    TRACE_LOG;
    eventLoop->poller->enableOneShot(events);
    update();
}

void
Channel::disableOneShot() {
    REQUIRE_(inEventLoop, "Channel is not in event loop");
    TRACE_LOG;
    eventLoop->poller->disableOneShot(events);
    update();
}

void
Channel::disableAll() {
    // FIXME
    TRACE_LOG;
    events = 0;
    update();
}

void
Channel::activate(const unsigned revents) {
    REQUIRE(eventLoop && eventLoop->poller);
    if(eventLoop->poller->isError(revents)) {
        // error event
        Log(TRACE) << "Activate ERROR callback with events " << revents;
        REQUIRE_(errorCallback, "errorCallback is not callable");
        errorCallback();
    } else if(eventLoop->poller->isClose(revents)) {
        // close event
        Log(TRACE) << "Activate CLOSE callback with events" << revents;
        REQUIRE_(closeCallback, "closeCallback is not callable");
        closeCallback();
    } else {
        // read event
        if(eventLoop->poller->isRead(revents)) {
            if(isRead()) {
                Log(TRACE) << "Activate READ callback with events " << revents;
                REQUIRE_(readCallback, "readCallback is not callable");
                readCallback();
            } else {
                Log(WARN) << "READ callback is not available";
            }
        }
        // write event
        if(eventLoop->poller->isWrite(revents)) {
            if(isWrite()) {
                Log(TRACE) << "Activate WRITE callback with events" << revents;
                REQUIRE_(writeCallback, "writeCallback is not callable");
                writeCallback();
            } else {
                Log(WARN) << "WRITE callback is not available";
            }
        }
    }
    if(updateAfterActivate) update();
}

void
Channel::update() {
    TRACE_LOG;
    eventLoop->updateChannel(shared_from_this());
}

void
Channel::remove() {
    TRACE_LOG;
    if(inEventLoop) eventLoop->removeChannel(shared_from_this());
}
}
}
