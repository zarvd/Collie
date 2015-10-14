#ifndef COLLIE_BASE_EVENT_CHANNEL_H
#define COLLIE_BASE_EVENT_CHANNEL_H

#include <functional>
#include <memory>


namespace Collie { namespace Base { namespace Event {

class EventLoop;

class Channel {
public:
    using EventCallback = std::function<void(const unsigned &)>;

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

    bool isNoneEvent() const;
    bool isRead() const ;
    bool isWrite() const;

    void enableRead();
    void disableRead();
    void enableWrite();
    void disableWrite();
    void disableAll();

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

}}}

#endif /* COLLIE_BASE_EVENT_CHANNEL_H */
