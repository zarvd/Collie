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
ThreadPool::pushChannel(std::shared_ptr<Channel> channel) {
    std::lock_guard<std::mutex> lock(channelMtx);
    channels.push_back(channel);
    channelCondition.notify_all();
}

void
ThreadPool::startEventLoop() {
}

void
ThreadPool::runInThread() {
    std::shared_ptr<EventLoop> eventLoop(new EventLoop);
    std::deque<std::shared_ptr<Channel> > channelsInThisThread;
    while(true) {
        {
            std::unique_lock<std::mutex> lock(channelMtx);
            channelCondition.wait(lock);

            channelsInThisThread.swap(channels);
        }
        for(auto channel : channelsInThisThread) {
            channel->setEventLoop(eventLoop);
            channel->update();
        }
        channelsInThisThread.clear();
        eventLoop->loopNonBlocking();
    }
}

}}
