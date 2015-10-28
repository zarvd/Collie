#include "../../include/event/ThreadPool.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"


namespace Collie { namespace Event {

ThreadPool::ThreadPool(const unsigned threadNum) :
    terminate(false) {
    for(unsigned i = 0; i < threadNum; ++ i) {
        threadPool.push_back(std::thread(&ThreadPool::runInThread, this));
    }
}

ThreadPool::~ThreadPool() {
    if( ! terminate) shutDown();
}

void
ThreadPool::shutDown() {
    {
        // send terminate signal to threads
        std::lock_guard<std::mutex> lock(channelMtx);

        terminate = true;
    }
    for(auto & thread : threadPool) {
        thread.join();
    }
}

void
ThreadPool::pushChannel(std::unique_ptr<Channel> channel) {
    std::lock_guard<std::mutex> lock(channelMtx);
    channels.push_back(std::move(channel));
    channelCondition.notify_all();
}

void
ThreadPool::startEventLoop() {
}

void
ThreadPool::runInThread() {
    std::shared_ptr<EventLoop> eventLoop(new EventLoop);
}

}}
