#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <memory>
#include <map>


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
    void updateChannel(const int fd);
    void removeChannel(const int fd);

private:
    typedef std::map<int, std::shared_ptr<Channel> > ChannelMap;

    std::unique_ptr<EPoller> poller;
    std::shared_ptr<ChannelMap> channels;
    bool isLooping;
};

}}

#endif /* EVENTLOOP_H */
