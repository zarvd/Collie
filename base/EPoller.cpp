#include "EPoller.hpp"


namespace MiniHttp { namespace Base {

EPoller::EPoller() :
    MaxEvent(64),
    eventQueue(new Event[MaxEvent]),
    eventNum(0) {
    create();
}

EPoller::EPoller(const unsigned& MaxEvent) :
    MaxEvent(MaxEvent),
    eventQueue(new Event[MaxEvent]),
    eventNum(0) {
    create();
}

EPoller::~EPoller() {}

Status EPoller::create() {
    epollFd = epoll_create1(0);
    if(epollFd == -1) {
        const std::string errMsg = strerror(errno);
        Log(ERROR) << "EPoll create failed: " << errMsg;
        return Status::FAIL;
    } else {
        return Status::SUCCESS;
    }
}


Status EPoller::insert(const int& fd, const int& events){
    Event event;
    event.events = events;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
    if(ret == -1) {
        Log(ERROR) << "EPoll add ctl failed: " << getErr();
        return Status::FAIL;
    }
    return Status::SUCCESS;
}

Status EPoller::modify(const int& fd, const int& events) {
    Event event;
    event.events = events;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event);
    if(ret == -1) {
        Log(ERROR) << "EPoll mod ctl failed: " << getErr();
        return Status::FAIL;
    }
    return Status::SUCCESS;
}

Status EPoller::remove(const int& fd) {
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL);
    if(ret == -1) {
        Log(ERROR) << "EPoll del ctl failed: " << getErr();
        return Status::FAIL;
    }
    return Status::SUCCESS;
}

void EPoller::wait(const unsigned& timeout) {
    eventNum = epoll_wait(epollFd, eventQueue.get(), MaxEvent, timeout);
    if(eventNum == -1) {
        Log(ERROR) << "EPoll wait failed: " << getErr();
    }
}

std::shared_ptr<EPoller::Event> EPoller::getEvents() const {
    return eventQueue;
}
}}
