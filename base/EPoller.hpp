#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h>
#include "../httpd.hpp"


enum EventType {
    Read = EPOLLIN,
    Write = EPOLLOUT,
    Error = EPOLLERR,
    Close = EPOLLHUP,
    OngShot = EPOLLONESHOT,
    EdgeTriggered = EPOLLET,
    Priority = EPOLLPRI
};

class EPoller {
public:
    typedef struct epoll_event Event;

    EPoller();
    explicit EPoller(const unsigned&);
    EPoller(const EPoller&) = delete;
    EPoller& operator=(const EPoller&) = delete;
    ~EPoller();

    Status insert(const int&, const int&);
    Status modify(const int&, const int&);
    Status remove(const int&);
    void wait(const unsigned& timeout = -1);
    std::shared_ptr<Event> getEvents() const;

    const unsigned MaxEvent;

private:
    Status create();

    int epollFd;
    std::shared_ptr<Event> eventQueue;
    int eventNum;
};


#endif /* EPOLLER_H */
