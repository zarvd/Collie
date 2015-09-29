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
    explicit EPoller(const unsigned&);
    ~EPoller();

    Status create();
    Status insert(const int&, const EventType&, const eventHandler&);
    Status modify(const int&, const EventType&, const eventHandler&);
    Status remove(const int&);
    void loop(const unsigned& timeout = -1);

    const unsigned MaxEvent;

private:
    bool isCreated;
    bool isLooping;
    int epollFd;
    std::map<int, eventHandler> handlers;
};


#endif /* EPOLLER_H */
