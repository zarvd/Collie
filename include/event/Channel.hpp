#ifndef COLLIE_EVENT_CHANNEL_H
#define COLLIE_EVENT_CHANNEL_H

#include <functional>
#include <memory>

namespace Collie {
namespace Event {

class EventLoop;

class Channel : public std::enable_shared_from_this<Channel> {
public:
    using EventCallback = std::function<void()>;

    explicit Channel(const int fd); // channel control socket life
    Channel(const Channel &) = delete;
    Channel & operator=(const Channel &) = delete;
    ~Channel();

    // setter
    void setReadCallback(const EventCallback & cb) { readCallback = cb; }
    void setReadCallback(const EventCallback && cb) {
        readCallback = std::move(cb);
    }
    void setWriteCallback(const EventCallback & cb) { writeCallback = cb; }
    void setWriteCallback(const EventCallback && cb) {
        writeCallback = std::move(cb);
    }
    void setCloseCallback(const EventCallback & cb) { closeCallback = cb; }
    void setCloseCallback(const EventCallback && cb) {
        closeCallback = std::move(cb);
    }
    void setErrorCallback(const EventCallback & cb) { errorCallback = cb; }
    void setErrorCallback(const EventCallback && cb) {
        errorCallback = std::move(cb);
    }
    void setEventLoop(std::shared_ptr<EventLoop>);

    // getter
    int getFd() const { return fd; }
    int getEvents() const { return events; }
    std::shared_ptr<EventLoop> getEventLoop() const { return eventLoop; }

    bool isNoneEvent() const { return events == 0; }
    bool isRead() const;
    bool isWrite() const;

    void enableRead();
    void disableRead();
    void enableWrite();
    void disableWrite();
    void disableAll();

    void activate(const unsigned revents) const;
    void remove();
    void update();

private:
    bool inEventLoop; // whether eventLoop is setting up
    const int fd;     // file descriptor
    unsigned events;
    std::shared_ptr<EventLoop> eventLoop;
    EventCallback readCallback;
    EventCallback writeCallback;
    EventCallback closeCallback;
    EventCallback errorCallback;
};
}
}

#endif /* COLLIE_EVENT_CHANNEL_H */
