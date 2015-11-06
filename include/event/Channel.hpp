#ifndef COLLIE_EVENT_CHANNEL_H
#define COLLIE_EVENT_CHANNEL_H

#include <functional>
#include <memory>

namespace Collie {
namespace Event {

class EventLoop;

/**
 * Thread safe is not required
 * Channel owns the file descriptor(default), it will close the fd when
 * destructing.
 */
class Channel : public std::enable_shared_from_this<Channel> {
public:
    using Callback = std::function<void(std::shared_ptr<Channel>)>;
    using EventCallback = std::function<void()>;

    explicit Channel(const int fd); // channel control socket life
    Channel(const Channel &) = delete;
    Channel & operator=(const Channel &) = delete;
    ~Channel();

    // setter
    void setOwnFd(const bool own) { ownFd = own; }
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
    void setAfterSetLoopCallback(const Callback & cb) {
        afterSetLoopCallback = cb;
    }
    void setAfterSetLoopCallback(const Callback && cb) {
        afterSetLoopCallback = std::move(cb);
    }
    void setUpdateAfterActivate(const bool update) {
        updateAfterActivate = update;
    }

    // getter
    int getFd() const { return fd; }
    int getEvents() const { return events; }
    std::shared_ptr<Channel> getCopyWithoutEventLoop() const;
    std::shared_ptr<EventLoop> getEventLoop() const { return eventLoop; }

    // event
    bool isNoneEvent() const { return events == 0; }
    bool isRead() const;
    bool isWrite() const;

    void enableRead();
    void disableRead();
    void enableWrite();
    void disableWrite();
    void enableOneShot();
    void disableOneShot();
    void disableAll();

    void activate(const unsigned revents);
    void remove();
    void update();

private:
    bool ownFd;
    bool inEventLoop; // whether eventLoop is setting up
    const int fd;     // file descriptor
    unsigned events;
    std::shared_ptr<EventLoop> eventLoop;
    bool updateAfterActivate;
    EventCallback readCallback;
    EventCallback writeCallback;
    EventCallback closeCallback;
    EventCallback errorCallback;
    Callback afterSetLoopCallback;
};
}
}

#endif /* COLLIE_EVENT_CHANNEL_H */
