#ifndef COLLIE_POLL_EPOLLPOLLER_H
#define COLLIE_POLL_EPOLLPOLLER_H

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include "Poller.hpp"

namespace Collie {
namespace Poll {

class EPollPoller : public Poller {
public:
    using Event = struct epoll_event;

    explicit EPollPoller(const unsigned maxEvent);
    EPollPoller(const EPollPoller &) = delete;
    EPollPoller & operator=(const EPollPoller &) = delete;
    ~EPollPoller() override;

    void create() override;
    void insert(const int fd, const unsigned events) override;
    void modify(const int fd, const unsigned events) override;
    void remove(const int fd) override;
    void poll(PollCallback cb, const int timeout = -1) override;

    enum class Type : unsigned {
        Read = EPOLLIN,
        Write = EPOLLOUT,
        Error = EPOLLERR,
        Close = EPOLLHUP,
        OngShot = EPOLLONESHOT,
        EdgeTriggered = EPOLLET,
        Priority = EPOLLPRI
    };

    void enableRead(unsigned & events) const noexcept override {
        events |= (unsigned)Type::Read;
    }
    void disableRead(unsigned & events) const noexcept override {
        events &= ~(unsigned)Type::Read;
    }
    void enableWrite(unsigned & events) const noexcept override {
        events |= (unsigned)Type::Write;
    }
    void disableWrite(unsigned & events) const noexcept override {
        events &= ~(unsigned)Type::Write;
    }
    bool isRead(const unsigned events) const noexcept override {
        return (unsigned)Type::Read & events;
    }
    bool isWrite(const unsigned events) const noexcept override {
        return (unsigned)Type::Write & events;
    }
    bool isError(const unsigned events) const noexcept override {
        return (unsigned)Type::Error & events;
    }
    bool isClose(const unsigned events) const noexcept override {
        return (unsigned)Type::Close & events;
    }

    // const unsigned MaxEvent;
private:
    bool isInit;
    int epollFd;
    std::unique_ptr<Event> revents;
};
}
}

#endif /* COLLIE_POLL_EPOLLPOLLER_H */
