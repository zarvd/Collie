#include "../../include/event/ThreadPool.hpp"
#include "../../include/event/EventLoop.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/Global.hpp"


namespace Collie { namespace Event {

ThreadPool::ThreadPool(const unsigned threadNum) :
    terminate(false) {
    Log(TRACE) << "ThreadPool is constructing";
    Log(TRACE) << "Thread pool create " << threadNum << " threads";
    for(unsigned i = 0; i < threadNum; ++ i) {
        threadPool.push_back(std::thread(&ThreadPool::runInThread, this));
    }
}

ThreadPool::~ThreadPool() {
    Log(TRACE) << "ThreadPool is destructing";
    if( ! terminate) shutDown();
}

void
ThreadPool::shutDown() {
    Log(TRACE) << "ThreadPool is shutting down";
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
}

void
ThreadPool::runInThread() {
    // one loop per thread
    std::shared_ptr<EventLoop> eventLoop(new EventLoop);
    std::vector<std::shared_ptr<Channel> > channelsInThisThread;
    while(true) {
        // Non blocking try lock mutex
        if(channelMtx.try_lock()) {
            // get new channel

            // terminate condition
            if(terminate && channels.empty()) exit(0);

            // FIXME dispatch channel more flexible
            channelsInThisThread.swap(channels);
            channelMtx.unlock();

            // insert channel into event loop
            for(auto & channel : channelsInThisThread) {
                channel->setEventLoop(eventLoop);
                channel->update();
            }

            // clear channels queue
            channelsInThisThread.clear();
        }
        // eventLoop->loop();
        eventLoop->loopNonBlocking();
    }
}

}}
