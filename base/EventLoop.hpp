#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <memory>
#include <vector>
#include "Channel.hpp"


class EventLoop {
public:
    EventLoop();
    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;
    ~EventLoop();
    void loop();
    void updateChannel(std::shared_ptr<Channel>);
    void removeChannel(std::shared_ptr<Channel>);
    bool hasChannel(std::shared_ptr<Channel>) const;

private:
    typedef std::vector<std::shared_ptr<Channel> > ChannelList;

    ChannelList channelList;
    bool isLooping;
};

#endif /* EVENTLOOP_H */
