#ifndef COLLIE_POLL_POLLPOLLER_H
#define COLLIE_POLL_POLLPOLLER_H

#include <memory>
#include "Poller.hpp"


namespace Collie { namespace Poll {

// TODO
class PollPoller : public Poller {
public:
    explicit PollPoller(const unsigned & maxEvent);
    PollPoller(const PollPoller &) = delete;
    PollPoller & operator=(const PollPoller &) = delete;
    ~PollPoller();

    void insert(const int fd, const unsigned events) override;
    void modify(const int fd, const unsigned events) override;
    void remove(const int fd) override;
    void poll(PollCallback cb, const int & timeout = -1) override;

    virtual void enableRead(unsigned & events) const noexcept override;
    virtual void disableRead(unsigned & events) const noexcept override;
    virtual void enableWrite(unsigned & events) const noexcept override;
    virtual void disableWrite(unsigned & events) const noexcept override;
    virtual bool isRead(const unsigned & events) const noexcept override;
    virtual bool isWrite(const unsigned & events) const noexcept override;
    virtual bool isError(const unsigned & events) const noexcept override;
    virtual bool isClose(const unsigned & events) const noexcept override;

private:
    void create() override;

    int pollFd;
};

}}

#endif /* COLLIE_POLL_POLLPOLLER_H */
