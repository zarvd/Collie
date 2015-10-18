#include "../../include/Global.hpp"
#include "../../include/poll/EPollPoller.hpp"


namespace Collie { namespace Poll {

EPollPoller::EPollPoller(const unsigned& maxEvent) :
    Poller(maxEvent),
    revents(new Event[MaxEvent]) {
    Log(TRACE) << "EPoller constructing";
    create();
}

EPollPoller::~EPollPoller() {
    Log(TRACE) << "EPoller destructing";
}

void
EPollPoller::create() {
    epollFd = epoll_create1(0);
    Log(TRACE) << "EPoller create new epoll " << epollFd;
    if(epollFd == -1) {
        Log(ERROR) << "EPoll create failed: " << Exception::getErr();
        THROW_SYS;
    }
}


void
EPollPoller::insert(const int fd, const unsigned events) {
    Log(TRACE) << "EPoller insert " << fd << " with events " << events;
    Event event;
    event.data.fd = fd;
    event.events = events;
    // event.events = events | (unsigned)Event::Type::EdgeTriggered;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
    if(ret == -1) {
        Log(ERROR) << "EPoll add ctl failed: " << Exception::getErr();
        THROW_SYS;
    }
}

void
EPollPoller::modify(const int fd, const unsigned events) {
    Log(TRACE) << "EPoller modify " << fd << " with events " << events;
    Event event;
    event.data.fd = fd;
    event.events = events;  // level-triggered
    // event.events = events | (unsigned)Event::Type::EdgeTriggered;
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event);
    if(ret == -1) {
        Log(ERROR) << "EPoll mod ctl failed: " << Exception::getErr();
        THROW_SYS;
    }
}

void
EPollPoller::remove(const int fd) {
    Log(TRACE) << "EPoller remove " << fd;
    // Since Linux 2.6.9, event can be specified as NULL when using EPOLL_CTL_DEL.
    const int ret = epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL);
    if(ret == -1) {
        Log(ERROR) << "EPoll del ctl failed: " << Exception::getErr();
        THROW_SYS;
    }
}

void
EPollPoller::poll(PollCallback cb, const int & timeout) {
    Log(TRACE) << "EPoller polling " << epollFd;
    int eventNum = epoll_wait(epollFd, revents.get(), MaxEvent, timeout);
    if(eventNum == -1) {
        Log(ERROR) << "EPoll wait failed: " << Exception::getErr();
        THROW_SYS;
    }

    Log(TRACE) << "EPoll get " << eventNum << " events";

    for(int idx = 0; idx < eventNum; ++ idx) {
        const Event & curEvent = revents.get()[idx];
        cb(curEvent.data.fd, curEvent.events);  // XXX
        // if(*channels->find(curEvent.data.fd) != *channels->end()) {
        //     Log(TRACE) << "Activating channel " << curEvent.data.fd;
        //     std::shared_ptr<Channel> channel = channels->at(curEvent.data.fd);
        //     // channel->activate(curEvent.events);
        // } else {
        //     Log(WARN) << "channel " << curEvent.data.fd << " is not existed";
        // }
    }
}

}}
