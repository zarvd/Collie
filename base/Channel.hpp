#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <memory>
#include "Event.hpp"
#include "Callback.hpp"


namespace MiniHttp { namespace Base {

class EventLoop;

class Channel {
public:

    Channel(std::shared_ptr<EventLoop>, const int & fd);
    Channel(const Channel&) = delete;
    Channel & operator=(const Channel &) = delete;
    ~Channel();
    void setReadCallback(const EventCallback & cb) { readCallback = cb; }
    void setWriteCallback(const EventCallback & cb) { writeCallback = cb; }
    void setCloseCallback(const EventCallback & cb) { closeCallback = cb; }
    void setErrorCallback(const EventCallback & cb) { errorCallback = cb; }
    int getFd() const { return fd; }
    int getEvents() const { return events; }
    bool isNoneEvent() const { return events == 0; }
    void enableRead() { Event::enableRead(events); }
    void disableRead() { Event::disableRead(events); }
    bool isRead() const { return Event::isRead(events); }
    void enableWrite() { Event::enableWrite(events); }
    void disableWrite() { Event::disableRead(events); }
    void disableAll() { events = 0; }
    bool isWrite() const { return Event::isWrite(events); }
    void activate(const unsigned & revents) const;

private:

    const int fd;  // file descriptor
    unsigned events;
    std::shared_ptr<EventLoop> eventLoop;
    EventCallback readCallback;
    EventCallback writeCallback;
    EventCallback closeCallback;
    EventCallback errorCallback;
};

}}

#endif /* CHANNEL_H */
