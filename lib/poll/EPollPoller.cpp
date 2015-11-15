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
    fd = epoll_create1(0);
    Log(TRACE) << "EPoller create new epoll " << fd;
    REQUIRE_SYS(fd != -1);

    isInit = true;
}

void
EPollPoller::insert(const int fd, const unsigned events) {
    REQUIRE_(isInit, "Epoller is not inited");
    Log(TRACE) << "EPoller " << this->fd << " insert " << fd << " with events "
               << events;
    Event event;
    event.data.fd = fd;
    event.events = events;
    const int ret = epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &event);
    REQUIRE_SYS(ret != -1);
}

void
EPollPoller::modify(const int fd, const unsigned events) {
    REQUIRE_(isInit, "Epoller is not inited");
    Log(TRACE) << "EPoller " << this->fd << " modify " << fd << " with events "
               << events;
    Event event;
    event.data.fd = fd;
    event.events = events;
    const int ret = epoll_ctl(this->fd, EPOLL_CTL_MOD, fd, &event);
    REQUIRE_SYS(ret != -1);
}

void
EPollPoller::remove(const int fd) {
    REQUIRE_(isInit, "Epoller is not inited");
    Log(TRACE) << "EPoller " << this->fd << " remove " << fd;
    // Since Linux 2.6.9, event can be specified as NULL when using
    // EPOLL_CTL_DEL.
    const int ret = epoll_ctl(this->fd, EPOLL_CTL_DEL, fd, NULL);
    REQUIRE_SYS(ret != -1);
}

void
EPollPoller::poll(PollCallback cb, const int timeout) {
    REQUIRE_(isInit, "Epoller is not inited");
    Log(TRACE) << "EPoller " << this->fd << " is polling ";
    int eventNum = epoll_wait(this->fd, revents.get(), MaxEvent, timeout);
    REQUIRE_SYS(eventNum != -1);

    Log(TRACE) << "Epoller " << this->fd << " get " << eventNum << " events";

    for(int idx = 0; idx < eventNum; ++idx) {
        const Event & curEvent = revents.get()[idx];
        REQUIRE_(cb, "PollCallback is not callable");
        cb(curEvent.data.fd, curEvent.events); // XXX
    }
}
}
}
