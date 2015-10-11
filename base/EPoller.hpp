#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h>
#include <vector>
#include <map>
#include "../Httpd.hpp"


namespace MiniHttp { namespace Base {

class Channel;

class EPoller {
public:
    typedef struct epoll_event PollEvent;
    typedef std::map<int, std::shared_ptr<Channel> > ChannelMap;

    EPoller();
    explicit EPoller(const unsigned &);
    EPoller(const EPoller &) = delete;
    EPoller & operator=(const EPoller &) = delete;
    ~EPoller();

    Status insert(const int fd, const unsigned events);
    Status modify(const int fd, const unsigned events);
    Status remove(const int fd);
    void poll(std::shared_ptr<ChannelMap> &, const unsigned & timeout = -1);

    const unsigned MaxEvent;

private:
    Status create();

    int epollFd;
    std::unique_ptr<PollEvent> revents;
};

}}

#endif /* EPOLLER_H */
