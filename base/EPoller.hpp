#ifndef EPOLLER_H
#define EPOLLER_H


#include "../httpd.hpp"
#include "sys/epoll.h"


class EPoller {
public:
    typedef struct epoll_event Event;
    typedef std::function<void(int)> eventHandler;
    enum EventType {
        In = EPOLLIN,
        Out = EPOLLOUT,
        Error = EPOLLERR,
        HangUp = EPOLLHUP,
        OngShot = EPOLLONESHOT,
        EdgeTriggered = EPOLLET,
        Priority = EPOLLPRI
    };

    EPoller();
    ~EPoller();

    Status create();
    void insert(const int&, const EventType&, eventHandler&);
    void remove();
    void modify();

    const unsigned MaxEvent;
private:
    int epollFd;
    Event event;
    Event * eventQueue;
};


#endif /* EPOLLER_H */
