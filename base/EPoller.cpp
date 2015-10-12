#include "EPoller.hpp"
#include "Channel.hpp"
#include "Event.hpp"


namespace MiniHttp { namespace Base {

EPoller::EPoller() :
    MaxEvent(64),
    revents(new PollEvent[MaxEvent]) {
    Log(TRACE) << "EPoller constructing";
    create();
}

EPoller::EPoller(const unsigned& MaxEvent) :
    MaxEvent(MaxEvent),
    revents(new PollEvent[MaxEvent]) {
    Log(TRACE) << "EPoller constructing";
    create();
}

EPoller::~EPoller() {
    Log(TRACE) << "EPoller destructing";
}

Status EPoller::create() {
    epollFd = epoll_create1(0);
    Log(TRACE) << "EPoller create new epoll " << epollFd;
    if(epollFd == -1) {
        const std::string errMsg = strerror(errno);
        Log(ERROR) << "EPoll create failed: " << errMsg;
        return Status::FAIL;
    } else {
        return Status::SUCCESS;
    }
}


Status EPoller::insert(const int fd, const unsigned events) {
    Log(TRACE) << "EPoller insert " << fd << " with events " << events;
    PollEvent event;
    event.data.fd = fd;
    event.events = events;
    // event.events = events | (unsigned)Event::Type::EdgeTriggered;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
    if(ret == -1) {
        Log(ERROR) << "EPoll add ctl failed: " << getErr();
        return Status::FAIL;
    }
    return Status::SUCCESS;
}

Status EPoller::modify(const int fd, const unsigned events) {
    Log(TRACE) << "EPoller modify " << fd << " with events " << events;
    PollEvent event;
    event.data.fd = fd;
    event.events = events;  // level-triggered
    // event.events = events | (unsigned)Event::Type::EdgeTriggered;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event);
    if(ret == -1) {
        Log(ERROR) << "EPoll mod ctl failed: " << getErr();
        return Status::FAIL;
    }
    return Status::SUCCESS;
}

Status EPoller::remove(const int fd) {
    Log(TRACE) << "EPoller remove " << fd;
    // Since Linux 2.6.9, event can be specified as NULL when using EPOLL_CTL_DEL.
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL);
    if(ret == -1) {
        Log(ERROR) << "EPoll del ctl failed: " << getErr();
        return Status::FAIL;
    }
    return Status::SUCCESS;
}

void EPoller::poll(std::shared_ptr<ChannelMap> & channels, const unsigned & timeout) {
    Log(TRACE) << "EPoller polling " << epollFd;
    int eventNum = epoll_wait(epollFd, revents.get(), MaxEvent, timeout);
    if(eventNum == -1) {
        Log(ERROR) << "EPoll wait failed: " << getErr();
    }

    Log(TRACE) << "EPoll get " << eventNum << " events";

    for(int idx = 0; idx < eventNum; ++ idx) {
        const PollEvent & curEvent = revents.get()[idx];
        if(*channels->find(curEvent.data.fd) != *channels->end()) {
            Log(TRACE) << "Activating channel " << curEvent.data.fd;
            std::shared_ptr<Channel> channel = channels->at(curEvent.data.fd);
            channel->activate(curEvent.events);
        } else {
            Log(WARN) << "channel " << curEvent.data.fd << " is not existed";
        }
    }
}

}}
