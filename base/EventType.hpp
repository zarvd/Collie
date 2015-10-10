#ifndef EVENT_H
#define EVENT_H

#include <sys/epoll.h>


namespace MiniHttp { namespace Base {

enum EventType {
    Read = EPOLLIN,
    Write = EPOLLOUT,
    Error = EPOLLERR,
    Close = EPOLLHUP,
    OngShot = EPOLLONESHOT,
    EdgeTriggered = EPOLLET,
    Priority = EPOLLPRI
};

}}


#endif /* EVENT_H */
