#ifndef COLLIE_EVENT_EVENTLOOPTHREADPOOL_H_
#define COLLIE_EVENT_EVENTLOOPTHREADPOOL_H_

#include <functional>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>
#include <map>

namespace collie {
namespace event {

class Channel;
class EventLoop;

/**
 * One loop per thread
 */
class EventLoopThreadPool
    : public std::enable_shared_from_this<EventLoopThreadPool> {
 public:
  EventLoopThreadPool(const EventLoopThreadPool &) = delete;
  EventLoopThreadPool &operator=(const EventLoopThreadPool &) = delete;
  ~EventLoopThreadPool();

  void StartLoop(std::vector<std::shared_ptr<Channel> > base_channels);
  static void PushChannel(std::shared_ptr<Channel> channel);
  static std::shared_ptr<EventLoop> GetCurrentEventLoop();
  static std::shared_ptr<EventLoopThreadPool> GetEventLoopThreadPool(
      const size_t thread_num = 4) {
    static auto global_pool = std::shared_ptr<EventLoopThreadPool>(
        new EventLoopThreadPool(thread_num));
    return global_pool;
  }
  const size_t kThreadNum;

 private:
  struct ChannelQueue {
    std::vector<std::shared_ptr<Channel> > channels;
    std::mutex mtx;
  };
  EventLoopThreadPool(const size_t thread_num);  // Sigleton
  void RunInThread();
  std::shared_ptr<ChannelQueue> GetNextQueue();

  std::vector<std::thread> workers_;
  std::mutex eventloop_thread_mtx_;
  std::vector<std::shared_ptr<Channel> > base_channels_;
  std::map<std::thread::id, std::shared_ptr<EventLoop> > eventloops_;
  std::vector<std::shared_ptr<ChannelQueue> > channel_queues_;
  std::atomic<bool> terminate_;
};
}
}

#endif /* COLLIE_EVENT_EVENTLOOPTHREADPOOL_H_ */
