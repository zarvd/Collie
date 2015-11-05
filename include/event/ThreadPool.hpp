#ifndef COLLIE_EVENT_THREADPOOL_H
#define COLLIE_EVENT_THREADPOOL_H

#include <functional>
#include <thread>
#include <mutex>
#include <vector>

namespace Collie {
namespace Event {

class EventLoop;
class Channel;

class ThreadPool {
public:
    explicit ThreadPool(const int threadNum);
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool & operator=(const ThreadPool &) = delete;
    ~ThreadPool();

    void pushChannel(std::shared_ptr<Channel>);
    void shutDown();
    void start(std::shared_ptr<Channel> baseChannel = nullptr);

private:
    void runInThread(std::shared_ptr<Channel> baseChannel = nullptr);

    const int threadNum;
    std::mutex channelMtx;
    std::vector<std::thread> threadPool;
    std::vector<std::shared_ptr<Channel> > channels;
    bool terminate;
};
}
}

#endif /* COLLIE_EVENT_THREADPOOL_H */
