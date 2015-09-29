#include "EPoller.hpp"


EPoller::EPoller() :
    MaxEvent(64), isCreated(false), isLooping(false), epollFd(0) {}

EPoller::EPoller(const unsigned& MaxEvent) :
    MaxEvent(MaxEvent), isCreated(false), isLooping(false), epollFd(0) {}

EPoller::~EPoller() {}

Status EPoller::create() {
    epollFd = epoll_create1(0);
    if(epollFd == -1) {
        Log(logLevel::Warn) << "EPoll create fail";
        return Status::Fail;
    } else {
        return Status::Success;
    }
}

Status EPoller::insert(const int& fd, const EPoller::EventType& events, const eventHandler& handler){
    if( ! isCreated) {
        Log(logLevel::Warn) << "EPolle is not created";
        return Status::Fail;
    }

    Event event;
    event.events = events;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
    if(ret == -1) {
        Log(logLevel::Warn) << "EPoll add ctl fail";
        return Status::Fail;
    }
    handlers.insert({fd, handler});
    return Status::Success;
}

Status EPoller::modify(const int& fd, const EPoller::EventType& events, const eventHandler& handler) {
    if( ! isCreated) {
        Log(logLevel::Warn) << "EPolle is not created";
        return Status::Fail;
    }

    Event event;
    event.events = events;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event);
    if(ret == -1) {
        Log(logLevel::Warn) << "EPoll mod ctl fail";
        return Status::Fail;
    }
    handlers[fd] = handler;
    return Status::Success;
}

Status EPoller::remove(const int& fd) {
    if( ! isCreated) {
        Log(logLevel::Warn) << "EPolle is not created";
        return Status::Fail;
    }

    const int ret = epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL);
    if(ret == -1) {
        Log(logLevel::Warn) << "EPoll del ctl fail";
        return Status::Fail;
    }
    handlers.erase(fd);
    return Status::Success;
}

void EPoller::loop(const unsigned& timeout) {
    if( ! isCreated) {
        Log(logLevel::Warn) << "EPoll is not created";
        return;
    }

    Event * eventQueue = new Event[MaxEvent];
    while(true) {
        isLooping = true;
        int n = epoll_wait(epollFd, eventQueue, MaxEvent, timeout);
        if(n == -1) {
            Log(logLevel::Warn) << "EPoll wait fail";
            return;
        }

        for(int idx = 0; idx < n; ++ idx) {
            Event& event = eventQueue[idx];
            handlers.at(event.data.fd)(event.data.fd);
        }
    }

    delete[] eventQueue;
}
