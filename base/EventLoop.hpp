#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <memory>
#include <unordered_set>


namespace MiniHttp { namespace Base {

class Channel;

class EPoller;

class EventLoop {
public:
    EventLoop();
    EventLoop(const EventLoop &) = delete;
    EventLoop & operator=(const EventLoop &) = delete;
    ~EventLoop();
    void loop();
    void updateChannel(std::shared_ptr<Channel>);
    void removeChannel(std::shared_ptr<Channel>);
    bool hasChannel(std::shared_ptr<Channel>) const;

private:
    typedef std::unordered_set<std::shared_ptr<Channel> > ChannelList;

    std::unique_ptr<EPoller> poller;
    ChannelList channelList;
    bool isLooping;
};

}}

#endif /* EVENTLOOP_H */
