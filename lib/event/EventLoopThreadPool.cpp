#include "../../include/event/EventLoopThreadPool.hpp"
#include "../../include/event/Channel.hpp"
#include "../../include/event/EventLoop.hpp"

namespace Collie {
namespace Event {

EventLoopThreadPool::EventLoopThreadPool(const size_t threadNum)
    : threadNum(threadNum) {}

void
EventLoopThreadPool::startLoop(
    std::vector<std::shared_ptr<Channel>> baseChannel) {
    if(!runInThread) {
        runInThread = [this, baseChannel] {
            std::shared_ptr<EventLoop> eventLoop(new EventLoop);
            for(auto channel : baseChannel) {
                auto newChannel = channel->getCopyWithoutEventLoop();
                newChannel->setEventLoop(eventLoop);
                newChannel->update();
            }

            while(true) {
                std::shared_ptr<Channel> channel;
                {
                    std::unique_lock<std::mutex> lock(this->mtx);
                    // XXX it will block event loop
                    condition.wait(lock, [this] {
                        return terminate || !channels.empty();
                    });
                    if(terminate && channels.empty()) return; // exit

                    channel = channels.front();
                    channels.pop();
                }
                channel->setEventLoop(eventLoop);
                channel->update();
                eventLoop->loopOne();
            }
        };
    }
}

void
EventLoopThreadPool::pushChannel(std::shared_ptr<Channel> channel) {
    std::unique_lock<std::mutex> lock(mtx);
    channels.push(channel);
    condition.notify_all();
}
}
}
