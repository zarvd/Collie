#ifndef COLLIE_EVENT_EVENTLOOPTHREADPOOL_H_
#define COLLIE_EVENT_EVENTLOOPTHREADPOOL_H_

#include <functional>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>

namespace collie {
namespace event {

class Channel;

/**
 * One loop per thread
 */
class EventLoopThreadPool {
 public:
  struct EventLoopThread {
    std::vector<std::shared_ptr<Channel>> channels;
    std::mutex mtx;
  };

  explicit EventLoopThreadPool(const size_t thread_num) noexcept;
  EventLoopThreadPool(const EventLoopThreadPool &) = delete;
  EventLoopThreadPool &operator=(const EventLoopThreadPool &) = delete;
  ~EventLoopThreadPool() noexcept;

  void StartLoop(std::vector<std::shared_ptr<Channel>> base_channels) noexcept;
  void PushChannel(std::shared_ptr<Channel> channel);
  std::shared_ptr<EventLoopThread> GetNextLoop();

  const size_t kThreadNum;

 private:
  void RunInThread();

  std::vector<std::thread> workers_;
  std::mutex eventloop_thread_mtx_;
  std::vector<std::shared_ptr<Channel>> base_channels_;
  std::vector<std::shared_ptr<EventLoopThread>> eventloop_threads_;
  std::atomic<bool> terminate_;
};
}
}

#endif /* COLLIE_EVENT_EVENTLOOPTHREADPOOL_H_ */
