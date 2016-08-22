#pragma once

#include <collie/Poller.h>
#include <atomic>

namespace collie {

class EPollPoller final : public Poller {
 public:
  EPollPoller();
  ~EPollPoller() override;

  // void Init() override;
  // void Destroy() override;
  void Poll(const PollCallback&, const int timeout = -1,
            int max_event = -1) const override;
  void Insert(const unsigned fd, const EventType&) const override;
  void Update(const unsigned fd, const EventType&) const override;
  void Remove(const unsigned fd) const override;

  unsigned ToEvents(const EventType&) const noexcept override;
  EventType ToEventType(const unsigned events) const noexcept override;
  unsigned MaxEvent() const noexcept override { return max_event; }
  void SetMaxEvent(unsigned num) noexcept override { max_event = num; }

 private:
  int efd;
  std::atomic<int> max_event;
};
}
