#ifndef COLLIE_EVENT_EVENTLOOPTHREADPOOL_H
#define COLLIE_EVENT_EVENTLOOPTHREADPOOL_H

#include <functional>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>

namespace Collie {
namespace Event {

class Channel;

/**
 * One loop per thread
 */
class EventLoopThreadPool {
public:
    struct EventLoopThread {
        std::vector<std::shared_ptr<Channel>> channels;
        std::mutex mtx;
    };

    explicit EventLoopThreadPool(const size_t threadNum);
    EventLoopThreadPool(const EventLoopThreadPool &) = delete;
    EventLoopThreadPool & operator=(const EventLoopThreadPool &) = delete;
    ~EventLoopThreadPool();

    size_t getThreadNum() const { return threadNum; }

    void startLoop(std::vector<std::shared_ptr<Channel>> baseChannel);
    void pushChannel(std::shared_ptr<Channel> channel);
    std::shared_ptr<EventLoopThread> getNextLoop();

private:
    void runInThread();

    const size_t threadNum;
    std::vector<std::thread> workers;
    std::mutex eventLoopThreadMtx;
    std::vector<std::shared_ptr<Channel>> baseChannel;
    std::vector<std::shared_ptr<EventLoopThread>> eventLoopThreads;
    std::atomic<bool> terminate;
};
}
}

#endif /* COLLIE_EVENT_EVENTLOOPTHREADPOOL_H */
