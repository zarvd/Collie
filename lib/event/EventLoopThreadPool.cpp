#include "../../include/Global.hpp"
#include "../../include/event/EventLoopThreadPool.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/EventLoop.hpp"

namespace Collie {
namespace Event {

EventLoopThreadPool::EventLoopThreadPool(const size_t threadNum)
    : threadNum(threadNum) {}

EventLoopThreadPool::~EventLoopThreadPool() {}

void
EventLoopThreadPool::startLoop(
    std::vector<std::shared_ptr<Channel>> baseChannel) {
    this->baseChannel = baseChannel;
    for(size_t i = 1; i < threadNum; ++i) {
        workers.emplace_back(&EventLoopThreadPool::runInThread, this);
    }
    runInThread();
}

std::shared_ptr<EventLoopThreadPool::EventLoopThread>
EventLoopThreadPool::getNextLoop() {
    std::lock_guard<std::mutex> lock(eventLoopThreadMtx);
    static auto it = eventLoopThreads.begin();
    if(it == eventLoopThreads.end()) {
        it = eventLoopThreads.begin();
    }
    return *(it++);
}

void
EventLoopThreadPool::pushChannel(std::shared_ptr<Channel> channel) {
    // insert to NEXT loop
    auto loop = getNextLoop();
    Log(ERROR) << "get loop";
    {
        std::lock_guard<std::mutex> lock(loop->mtx);
        loop->channels.push_back(channel);
    }
    Log(ERROR) << "pushed channel";
}

void
EventLoopThreadPool::runInThread() {
    auto eventLoopThread = std::make_shared<EventLoopThread>();
    {
        std::lock_guard<std::mutex> lock(eventLoopThreadMtx);
        eventLoopThreads.push_back(eventLoopThread);
    }

    // one loop per thread
    auto eventLoop = std::make_shared<EventLoop>();

    // insert base channels to loop
    for(auto channel : baseChannel) {
        auto newChannel = channel->getCopyWithoutEventLoop();
        newChannel->setEventLoop(eventLoop);
        newChannel->update();
    }

    // loop
    while(true) {
        std::vector<std::shared_ptr<Channel>> channels;
        // get new channel or terminate
        // Non blocking
        if(eventLoopThread->mtx.try_lock()) {
            if(terminate || !eventLoopThread->channels.empty()) {
                if(terminate && eventLoopThread->channels.empty())
                    return; // exit
                std::swap(eventLoopThread->channels, channels);
            }
            eventLoopThread->mtx.unlock();
        }
        // insert channel
        for(auto channel : channels) {
            channel->setEventLoop(eventLoop);
            channel->update();
        }
        // block
        eventLoop->loopOne();
    }
}
}
}
