#ifndef COLLIE_BASE_POLL_POLLER_H
#define COLLIE_BASE_POLL_POLLER_H

#include <functional>


namespace Collie { namespace Base { namespace Poll {

/**
 * Abstract class
 */
class Poller {
public:
    typedef std::function<void(const unsigned fd, const unsigned revents)> PollCallback;

    explicit Poller(const unsigned & maxEvent) :
        MaxEvent(maxEvent), fd(-1) {}
    Poller(const Poller &) = delete;
    Poller & operator=(const Poller &) = delete;
    virtual ~Poller() = 0;

    virtual void insert(const int fd, const unsigned events);
    virtual void modify(const int fd, const unsigned events);
    virtual void remove(const int fd);
    virtual void poll(PollCallback cb, const int & timeout = -1);

    virtual void enableRead(unsigned & events);
    virtual void disableRead(unsigned & events);
    virtual void enableWrite(unsigned & events);
    virtual void disableWrite(unsigned & events);
    virtual bool isRead(const unsigned & events);
    virtual bool isWrite(const unsigned & events);
    virtual bool isError(const unsigned & events);
    virtual bool isClose(const unsigned & events);

    const unsigned MaxEvent;

protected:
    virtual void create();

    int fd;
};

}}}


#endif /* COLLIE_BASE_POLL_POLLER_H */
