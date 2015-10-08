#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include "EPoller.hpp"


class EventLoop;

class Channel final {
 public:
    typedef std::function<void()> EventCallback;

    Channel(std::shared_ptr<EventLoop>, const int&);
    Channel(const Channel&) = delete;  // non-copyable
    Channel& operator=(const Channel&) = delete;
    ~Channel();
    void setReadCallback(EventCallback& cb) { readCallback = cb; }
    void setReadCallback(EventCallback&& cb) { readCallback = std::move(cb); }
    void setWriteCallback(EventCallback& cb) { writeCallback = cb; }
    void setWriteCallback(EventCallback&& cb) { writeCallback = std::move(cb); }
    void setCloseCallback(EventCallback& cb) { closeCallback = cb; }
    void setCloseCallback(EventCallback&& cb) { closeCallback = std::move(cb); }
    void setErrorCallback(EventCallback& cb) { errorCallback = cb; }
    void setErrorCallback(EventCallback&& cb) { errorCallback = std::move(cb); }
    int getFd() const { return fd; }
    int getEvents() const { return events; }
    bool isNoneEvent() const { return events == 0; }
    void enableRead() { events |= EventType::Read; update(); }
    void disableRead() { events &= ~EventType::Read; update(); }
    bool isRead() const { return events & EventType::Read; }
    void enableWrite() { events |= EventType::Write; update(); }
    void disableWrite() { events &= ~EventType::Write; update(); }
    void disableAll() { events = 0; update(); };
    bool isWrite() const { return events & EventType::Write; }
    void remove();

 private:
    void update();

    const int fd;  // file descriptor
    int events;
    std::shared_ptr<EventLoop> eventLoop;
    bool inLoop;
    EventCallback readCallback;
    EventCallback writeCallback;
    EventCallback closeCallback;
    EventCallback errorCallback;
};

#endif /* CHANNEL_H */
