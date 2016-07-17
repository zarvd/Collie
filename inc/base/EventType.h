#ifndef COLLIE_BASE_EVENT_TYPE_H_
#define COLLIE_BASE_EVENT_TYPE_H_

namespace collie {
namespace base {

class EventType {
 public:
  constexpr EventType() noexcept {}
  ~EventType() noexcept {}

  bool IsRead() const noexcept { return is_read; }
  bool IsWrite() const noexcept { return is_write; }
  bool IsError() const noexcept { return is_error; }
  bool IsClose() const noexcept { return is_close; }
  bool IsEdgeTriggerd() const noexcept { return is_et; }
  bool IsUrgentRead() const noexcept { return is_urgent; }
  bool IsOneShot() const noexcept { return is_oneshot; }

  void SetRead(bool is_enable) noexcept { is_read = is_enable; }
  void SetWrite(bool is_enable) noexcept { is_write = is_enable; }
  void SetError(bool is_enable) noexcept { is_error = is_enable; }
  void SetClose(bool is_enable) noexcept { is_close = is_enable; }
  void SetEdgeTriggeder(bool is_enable) noexcept { is_et = is_enable; }
  void SetUrgetnRead(bool is_enable) noexcept { is_urgent = is_enable; }
  void SetOneShot(bool is_enable) noexcept { is_oneshot = is_enable; }

 private:
  bool is_read = false;
  bool is_write = false;
  bool is_error = false;
  bool is_close = false;
  bool is_et = false;  // edge triggered
  bool is_urgent = false;
  bool is_oneshot = false;
};
}
}

#endif /* COLLIE_BASE_EVENT_TYPE_H_ */
