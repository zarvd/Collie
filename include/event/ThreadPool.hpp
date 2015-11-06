#ifndef COLLIE_EVENT_THREADPOOL_H
#define COLLIE_EVENT_THREADPOOL_H

#include <functional>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <atomic>
#include "../Global.hpp"

namespace Collie {
namespace Event {

class ThreadPool {
public:
    using Task = std::function<void()>;

    explicit ThreadPool(const size_t threadNum);
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool & operator=(const ThreadPool &) = delete;
    ~ThreadPool();

    void shutDown();
    bool isShutDown() const { return terminate; }

    template <class Cb, class... Args>
    auto enqueue(Cb && cb, Args &&... args)
        -> std::future<typename std::result_of<Cb(Args...)>::type>;

private:
    void runInThread();

    const size_t threadNum;
    std::mutex mtx;
    std::condition_variable condition;
    std::vector<std::thread> workers;
    std::queue<Task> tasks;

    std::atomic<bool> terminate;
};

template <class Cb, class... Args>
auto ThreadPool::enqueue(Cb && cb, Args &&... args)
    -> std::future<typename std::result_of<Cb(Args...)>::type> {
    using returnType = typename std::result_of<Cb(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<returnType()>>(
        std::bind(std::forward<Cb>(cb), std::forward<Args>(args)...));

    std::future<returnType> res = task->get_future();
    {
        std::lock_guard<std::mutex> lock(mtx);

        // don't allow enqueueing after stopping the pool
        if(terminate) {
            THROW_INVALID_ARGUMENT_("enqueue on stopped ThreadPool");
        }
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}
}
}

#endif /* COLLIE_EVENT_THREADPOOL_H */
