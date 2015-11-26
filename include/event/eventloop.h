#ifndef COLLIE_EVENT_EVENTLOOP_H_
#define COLLIE_EVENT_EVENTLOOP_H_

#include <memory>
#include <map>

namespace collie {

namespace poll {
class Poller;
}

namespace event {

class Channel;

/**
 * Thread safe is not required
 */
class EventLoop : public std::enable_shared_from_this<EventLoop> {
 public:
  EventLoop();
  explicit EventLoop(std::unique_ptr<poll::Poller> poller);
  EventLoop(const EventLoop &) = delete;
  EventLoop &operator=(const EventLoop &) = delete;
  ~EventLoop();

  void LoopOne();  // main method, TODO setting timeout
  void Loop();     // main method, TODO setting timeout
  void LoopNonBlocking();
  void UpdateChannel(std::shared_ptr<Channel>);  // update or insert
  void RemoveChannel(std::shared_ptr<Channel>);
  bool HasChannel(std::shared_ptr<Channel>) const;

  const std::unique_ptr<poll::Poller> kPoller;

 private:
  using ChannelMap = std::map<int, std::shared_ptr<Channel>>;

  void PollCallback(const int fd, const unsigned revents);

  ChannelMap channel_map_;
};
}
}

#endif /* COLLIE_EVENT_EVENTLOOP_H_ */
