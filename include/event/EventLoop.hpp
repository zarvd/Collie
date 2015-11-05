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

class EventLoop {
public:
    EventLoop();
    EventLoop(const EventLoop &) = delete;
    EventLoop & operator=(const EventLoop &) = delete;
    ~EventLoop();

    void loop(); // main method
    void loopNonBlocking();
    void updateChannel(std::shared_ptr<Channel>); // update or insert
    void removeChannel(std::shared_ptr<Channel>);
    bool hasChannel(std::shared_ptr<Channel>) const;

    const std::unique_ptr<Poll::Poller> poller;
private:
    using ChannelMap = std::map<int, std::shared_ptr<Channel>>;

    void pollCallback(const unsigned fd, const unsigned revents);

    ChannelMap channels;
};
}
}

#endif /* COLLIE_EVENT_EVENTLOOP_H */
