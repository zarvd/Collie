#ifndef COLLIE_POLL_POLLER_H
#define COLLIE_POLL_POLLER_H

#include <functional>

namespace Collie {
namespace Poll {

/**
 * Abstract class
 */
class Poller {
public:
    using PollCallback =
        std::function<void(const unsigned fd, const unsigned revents)>;

    explicit Poller(const unsigned & maxEvent) noexcept : MaxEvent(maxEvent),
                                                          fd(-1) {}
    Poller(const Poller &) = delete;
    Poller & operator=(const Poller &) = delete;
    virtual ~Poller() = 0;

    virtual void create() = 0;
    virtual void insert(const int fd, const unsigned events) = 0;
    virtual void modify(const int fd, const unsigned events) = 0;
    virtual void remove(const int fd) = 0;
    virtual void poll(PollCallback cb, const int timeout = -1) = 0;

    virtual void enableRead(unsigned & events) const noexcept = 0;
    virtual void disableRead(unsigned & events) const noexcept = 0;
    virtual void enableWrite(unsigned & events) const noexcept = 0;
    virtual void disableWrite(unsigned & events) const noexcept = 0;
    virtual void enableOneShot(unsigned & events) const noexcept = 0;
    virtual void disableOneShot(unsigned & events) const noexcept = 0;
    virtual bool isRead(const unsigned events) const noexcept = 0;
    virtual bool isWrite(const unsigned events) const noexcept = 0;
    virtual bool isError(const unsigned events) const noexcept = 0;
    virtual bool isClose(const unsigned events) const noexcept = 0;

    const unsigned MaxEvent;

protected:
    int fd;
};
}
}

#endif /* COLLIE_POLL_POLLER_H */
