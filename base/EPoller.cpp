#include "EPoller.hpp"


EPoller::EPoller() : MaxEvent(64) {
    if(create() == Status::FAIL) {
        throw std::runtime_error("EPoll create failed");
    }
}

EPoller::EPoller(const unsigned& MaxEvent) : MaxEvent(MaxEvent) {
    if(create() == Status::FAIL) {
        throw std::runtime_error("EPoll create failed");
    }
}

EPoller::~EPoller() {}

Status EPoller::create() {
    epollFd = epoll_create1(0);
    if(epollFd == -1) {
        Log(WARN) << "EPoll create failed";
        return Status::FAIL;
    } else {
        return Status::SUCCESS;
    }
}


Status EPoller::insert(const int& fd, const EventType& events, const eventHandler& handler){
    Event event;
    event.events = events;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
    if(ret == -1) {
        Log(WARN) << "EPoll add ctl failed";
        return Status::FAIL;
    }
    handlers.insert({fd, handler});
    return Status::SUCCESS;
}

Status EPoller::modify(const int& fd, const EventType& events, const eventHandler& handler) {
    Event event;
    event.events = events;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event);
    if(ret == -1) {
        Log(WARN) << "EPoll mod ctl failed";
        return Status::FAIL;
    }
    handlers[fd] = handler;
    return Status::SUCCESS;
}

Status EPoller::remove(const int& fd) {
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL);
    if(ret == -1) {
        Log(WARN) << "EPoll del ctl failed";
        return Status::FAIL;
    }
    handlers.erase(fd);
    return Status::SUCCESS;
}

void EPoller::loop(const unsigned& timeout) {
    Event * eventQueue = new Event[MaxEvent];
    while(true) {
        int n = epoll_wait(epollFd, eventQueue, MaxEvent, timeout);
        if(n == -1) {
            Log(WARN) << "EPoll wait failed";
            return;
        }

        for(int idx = 0; idx < n; ++ idx) {
            Event& event = eventQueue[idx];
            handlers.at(event.data.fd)(event.data.fd);
        }
    }
    delete[] eventQueue;
}
