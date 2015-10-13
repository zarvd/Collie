#ifndef EVENT_H
#define EVENT_H

#include <sys/epoll.h>


namespace MiniHttp { namespace Base { namespace Event {

enum class Type : unsigned {
    Read = EPOLLIN,
    Write = EPOLLOUT,
    Error = EPOLLERR,
    Close = EPOLLHUP,
    OngShot = EPOLLONESHOT,
    EdgeTriggered = EPOLLET,
    Priority = EPOLLPRI
};

inline void enableRead(unsigned & events) {
    events |= (unsigned)Type::Read;
}

inline void disableRead(unsigned & events) {
    events &= ~(unsigned)Type::Read;
}

inline void enableWrite(unsigned & events) {
    events |= (unsigned)Type::Write;
}

inline void disableWrite(unsigned & events) {
    events &= ~(unsigned)Type::Write;
}

inline bool isRead(const unsigned & events) {
    return (unsigned)Type::Read & events;
}

inline bool isWrite(const unsigned & events) {
    return (unsigned)Type::Write & events;
}

inline bool isError(const unsigned & events) {
    return (unsigned)Type::Error & events;
}

inline bool isClose(const unsigned & events) {
    return (unsigned)Type::Close & events;
}

}}}

#endif /* EVENT_H */
