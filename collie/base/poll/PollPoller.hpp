#ifndef COLLIE_BASE_POLL_POLLPOLLER_H
#define COLLIE_BASE_POLL_POLLPOLLER_H

#include "Poller.hpp"


namespace Collie { namespace Base { namespace Poll {

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
    void poll(std::shared_ptr<ChannelMap>,
              const int & timeout = -1) override;

private:
    void create() override;

    int pollFd;
};

}}}

#endif /* COLLIE_BASE_POLL_POLLPOLLER_H */
