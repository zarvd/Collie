#ifndef COLLIE_EVENT_EVENTLOOP_H
#define COLLIE_EVENT_EVENTLOOP_H

#include <memory>
#include <map>

namespace Collie {

namespace Poll {
class Poller;
}

namespace Event {

class Channel;

/**
 * Thread safe is not required
 */
class EventLoop {
public:
    EventLoop();
    explicit EventLoop(std::unique_ptr<Poll::Poller> poller);
    EventLoop(const EventLoop &) = delete;
    EventLoop & operator=(const EventLoop &) = delete;
    ~EventLoop();

    void loopOne(); // main method, TODO setting timeout
    void loop(); // main method, TODO setting timeout
    void loopNonBlocking();
    void updateChannel(std::shared_ptr<Channel>); // update or insert
    void removeChannel(std::shared_ptr<Channel>);
    bool hasChannel(std::shared_ptr<Channel>) const;

    const std::unique_ptr<Poll::Poller> poller;

private:
    using ChannelMap = std::map<int, std::shared_ptr<Channel>>;

    void pollCallback(const int fd, const unsigned revents);

    ChannelMap channels;
};
}
}

#endif /* COLLIE_EVENT_EVENTLOOP_H */
