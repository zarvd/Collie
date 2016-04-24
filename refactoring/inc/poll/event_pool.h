#ifndef COLLIE_POLL_EVENT_POOL_H_
#define COLLIE_POLL_EVENT_POOL_H_

#include <memory>
#include <vector>

#ifdef __linux__
#include "epoll_poller.h"
#define POLLER EPollPoller
#else
#include "select_poller.h"
#define POLLER SelectPoller
#endif

namespace collie {

// Singleton
class EventPool : public NonCopyable {
 public:
  ~EventPool();

  void Loop() noexcept;
  void LoopOne() noexcept;
  void Init() noexcept;
  void Destroy() noexcept;
  void Insert(std::shared_ptr<Event>, Event::Type) noexcept;
  void Update(std::shared_ptr<Event>, Event::Type) noexcept;
  void Delete(std::shared_ptr<Event>) noexcept;
  unsigned GetEventNum() const noexcept { return events_.size(); }

  static EventPool& GetInstance() {
    static EventPool instance;
    return instance;
  }

 private:
  EventPool();

  std::vector<std::shared_ptr<Event> > events_;
};
}

#endif /* COLLIE_POLL_EVENT_POOL_H_ */
