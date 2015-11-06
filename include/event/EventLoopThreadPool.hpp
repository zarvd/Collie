#ifndef COLLIE_EVENT_EVENTLOOPTHREADPOOL_H
#define COLLIE_EVENT_EVENTLOOPTHREADPOOL_H

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace Collie {
namespace Event {

class Channel;

class EventLoopThreadPool {
public:
    using RunInThread = std::function<void()>;

    explicit EventLoopThreadPool(const size_t threadNum);
    EventLoopThreadPool(const EventLoopThreadPool &) = delete;
    EventLoopThreadPool & operator=(const EventLoopThreadPool &) = delete;
    ~EventLoopThreadPool();

    size_t getThreadNum() const { return threadNum; }

    void setRunInThread(const RunInThread & cb) { runInThread = cb; }
    void setRunInThread(const RunInThread && cb) {
        runInThread = std::move(cb);
    }
    void startLoop(std::vector<std::shared_ptr<Channel>> baseChannel);
    void pushChannel(std::shared_ptr<Channel> channel);

private:
    const size_t threadNum;
    std::mutex mtx;
    std::condition_variable condition;
    RunInThread runInThread;
    std::vector<std::thread> workers;
    std::queue<std::shared_ptr<Channel>> channels;
    std::atomic<bool> terminate;
};
}
}

#endif /* COLLIE_EVENT_EVENTLOOPTHREADPOOL_H */
