#include "../../include/event/ThreadPool.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/Global.hpp"

namespace Collie {
namespace Event {

ThreadPool::ThreadPool(const size_t threadNum)
    : threadNum(threadNum), terminate(false) {
    Log(TRACE) << "ThreadPool is constructing";
}

ThreadPool::~ThreadPool() {
    Log(TRACE) << "ThreadPool is destructing";
    if(!terminate) shutDown();
}

void
ThreadPool::shutDown() {
    Log(TRACE) << "ThreadPool is shutting down";
    {
        // send terminate signal to threads
        std::lock_guard<std::mutex> lock(mtx);

        terminate = true;
    }
    for(auto & worker : workers) {
        worker.join();
    }
}

void
ThreadPool::runInThread() {
    while(true) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            condition.wait(lock, [this] {
                return this->terminate || !this->tasks.empty();
            });

            if(terminate && tasks.empty()) return;  // exit
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}

}
}
