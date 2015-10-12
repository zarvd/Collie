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
    std::shared_ptr<EventLoop> const getEventLoop() { return eventLoop; }

    bool getInEventLoop() const { return inEventLoop; }
    void goInEventLoop();
    void goOutEventLoop();

    bool isNoneEvent() const { return events == 0; }
    bool isRead() const { return Event::isRead(events); }
    bool isWrite() const { return Event::isWrite(events); }

    void enableRead() { Event::enableRead(events); update(); }
    void disableRead() { Event::disableRead(events); update(); }
    void enableWrite() { Event::enableWrite(events); update(); }
    void disableWrite() { Event::disableWrite(events); update(); }
    void disableAll() { events = 0; update(); }

    void activate(const unsigned & revents) const;

    void remove() const;
    void update() const;

private:

    bool inEventLoop;
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
