#include "../../include/Global.hpp"
#include "../../include/poll/EPollPoller.hpp"

namespace Collie {
namespace Poll {

EPollPoller::EPollPoller(const unsigned maxEvent)
    : Poller(maxEvent), isInit(false), revents(new Event[MaxEvent]) {
    Log(TRACE) << "EPoller constructing";
}

EPollPoller::~EPollPoller() { Log(TRACE) << "EPoller destructing"; }

void
EPollPoller::create() {
    this->fd = epoll_create1(0);
    Log(TRACE) << "EPoller create new epoll " << this->fd;
    if(this->fd == -1) {
        THROW_SYS_("Epoller create failed");
    }
    isInit = true;
}

void
EPollPoller::insert(const int fd, const unsigned events) {
    if(!isInit) {
        THROW_INVALID_ARGUMENT_("Epoller is not inited");
    }
    Log(TRACE) << "EPoller " << this->fd << " insert " << fd << " with events "
               << events;
    Event event;
    event.data.fd = fd;
    event.events = events;
    const int ret = epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &event);
    if(ret == -1) {
        THROW_SYS_("Epoller " + std::to_string(this->fd) + " add ctl failed: ");
    }
}

void
EPollPoller::modify(const int fd, const unsigned events) {
    if(!isInit) {
        THROW_INVALID_ARGUMENT_("Epoller is not inited");
    }
    Log(TRACE) << "EPoller " << this->fd << " modify " << fd << " with events "
               << events;
    Event event;
    event.data.fd = fd;
    event.events = events;
    const int ret = epoll_ctl(this->fd, EPOLL_CTL_MOD, fd, &event);
    if(ret == -1) {
        THROW_SYS_("Epoller " + std::to_string(this->fd) + " mod ctl failed: ");
    }
}

void
EPollPoller::remove(const int fd) {
    if(!isInit) {
        THROW_INVALID_ARGUMENT_("Epoller is not inited");
    }
    Log(TRACE) << "EPoller " << this->fd << " remove " << fd;
    // Since Linux 2.6.9, event can be specified as NULL when using
    // EPOLL_CTL_DEL.
    const int ret = epoll_ctl(this->fd, EPOLL_CTL_DEL, fd, NULL);
    if(ret == -1) {
        THROW_SYS_("Epoller " + std::to_string(this->fd) + " rm ctl failed: ");
    }
}

void
EPollPoller::poll(PollCallback cb, const int timeout) {
    if(!isInit) {
        THROW_INVALID_ARGUMENT_("Epoller is not inited");
    }
    Log(TRACE) << "EPoller " << this->fd << " is polling ";
    int eventNum = epoll_wait(this->fd, revents.get(), MaxEvent, timeout);
    if(eventNum == -1) {
        THROW_SYS_("Epoller " + std::to_string(this->fd) + " wait failed: ");
    }

    Log(TRACE) << "Epoller " << this->fd << " get " << eventNum << " events";

    for(int idx = 0; idx < eventNum; ++idx) {
        const Event & curEvent = revents.get()[idx];
        if(cb) {
            cb(curEvent.data.fd, curEvent.events); // XXX
        } else {
            THROW_NOTFOUND_("PollCallback is not callable");
        }
    }
}
}
}
