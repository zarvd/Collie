#include "EPoller.hpp"


EPoller::EPoller() : MaxEvent(64), epollFd(0), eventQueue(nullptr) {}

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
