#include "EPoller.hpp"
#include "Channel.hpp"
#include "Event.hpp"


namespace MiniHttp { namespace Base {

EPoller::EPoller() :
    MaxEvent(64),
    events(MaxEvent) {
    create();
}

EPoller::EPoller(const unsigned& MaxEvent) :
    MaxEvent(MaxEvent),
    events(MaxEvent) {
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
    PollEvent event;
    event.events = events;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
    if(ret == -1) {
        Log(ERROR) << "EPoll add ctl failed: " << getErr();
        return Status::FAIL;
    }
    return Status::SUCCESS;
}

Status EPoller::modify(const int& fd, const int& events) {
    PollEvent event;
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

void EPoller::poll(std::shared_ptr<ChannelMap> & channels, const unsigned & timeout) {
    int eventNum = epoll_wait(epollFd, &*events.begin(), MaxEvent, timeout);
    if(eventNum == -1) {
        Log(ERROR) << "EPoll wait failed: " << getErr();
    }
    for(int idx = 0; idx < eventNum; ++ idx) {
        const PollEvent & curEvent = events[idx];
        auto & channel = channels->at(curEvent.data.fd);
        channel->activate(curEvent.events);
    }
}

}}
