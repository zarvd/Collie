#ifndef COLLIE_EVENT_THREADPOOL_H
#define COLLIE_EVENT_THREADPOOL_H

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <ctime>
#include <chrono>
#include <vector>


namespace Collie { namespace Event {

class EventLoop;
class Channel;

class ThreadPool {
public:
    explicit ThreadPool(const unsigned threadNum);
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool & operator=(const ThreadPool &) = delete;
    ~ThreadPool();

    void pushChannel(std::shared_ptr<Channel>);
    void shutDown();
    void startEventLoop();

private:
    void runInThread();

    std::mutex channelMtx;
    std::condition_variable channelCondition;
    std::chrono::milliseconds freshFrequency;
    std::vector<std::thread> threadPool;
    std::vector<std::shared_ptr<Channel> > channels;
    bool terminate;
};

}}

#endif /* COLLIE_EVENT_THREADPOOL_H */
