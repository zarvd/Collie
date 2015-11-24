#ifndef COLLIE_EVENT_CHANNEL_H_
#define COLLIE_EVENT_CHANNEL_H_

#include <functional>
#include <memory>

namespace collie {
class Descriptor;

namespace event {

class EventLoop;

class Channel : public std::enable_shared_from_this<Channel> {
 public:
  using Callback = std::function<void(std::shared_ptr<Channel>)>;
  using EventCallback = std::function<void()>;

  explicit Channel(std::shared_ptr<Descriptor>);
  Channel(const Channel &) = delete;
  Channel &operator=(const Channel &) = delete;
  ~Channel();

  // setter
  void set_read_callback(const EventCallback &cb) { read_callback_ = cb; }
  void set_read_callback(const EventCallback &&cb) {
    read_callback_ = std::move(cb);
  }
  void set_write_callback(const EventCallback &cb) { write_callback_ = cb; }
  void set_write_callback(const EventCallback &&cb) {
    write_callback_ = std::move(cb);
  }
  void set_close_callback(const EventCallback &cb) { close_callback_ = cb; }
  void set_close_callback(const EventCallback &&cb) {
    close_callback_ = std::move(cb);
  }
  void set_error_callback(const EventCallback &cb) { error_callback_ = cb; }
  void set_error_callback(const EventCallback &&cb) {
    error_callback_ = std::move(cb);
  }
  void set_eventloop(std::shared_ptr<EventLoop>);
  void set_after_set_loop_callback(const Callback &cb) {
    after_set_loop_callback_ = cb;
  }
  void set_after_set_loop_callback(const Callback &&cb) {
    after_set_loop_callback_ = std::move(cb);
  }
  void set_update_after_activate(const bool update) {
    update_after_activate_ = update;
  }

  // getter
  std::shared_ptr<Descriptor> descriptor() const { return descriptor_; }
  int events() const { return events_; }
  std::shared_ptr<Channel> GetCopyWithoutEventLoop() const;
  std::shared_ptr<EventLoop> eventloop() const { return eventloop_; }

  // event
  bool IsNoneEvent() const { return events_ == 0; }
  bool IsRead() const;
  bool IsWrite() const;

  void EnableRead();
  void DisableRead();
  void EnableWrite();
  void DisableWrite();
  void EnableOneShot();
  void DisableOneShot();
  void DisableAll();

  void Activate(const unsigned revents);
  void Remove();
  void Update();

 private:
  bool in_eventloop_;                       // whether eventLoop is setting up
  std::shared_ptr<Descriptor> descriptor_;  // file descriptor
  unsigned events_;
  std::shared_ptr<EventLoop> eventloop_;
  bool update_after_activate_;

  EventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback close_callback_;
  EventCallback error_callback_;
  Callback after_set_loop_callback_;
};
}
}

#endif /* COLLIE_EVENT_CHANNEL_H_ */
