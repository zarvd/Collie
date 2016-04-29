#ifndef COLLIE_EVENT_H_
#define COLLIE_EVENT_H_

namespace collie {

class Event {
 public:
  Event() noexcept {}
  ~Event() noexcept {}

  bool IsRead() const noexcept { return is_read_; }
  bool IsWrite() const noexcept { return is_write_; }
  bool IsError() const noexcept { return is_error_; }
  bool IsClose() const noexcept { return is_close_; }
  bool IsEdgeTriggerd() const noexcept { return is_et_; }
  bool IsUrgentRead() const noexcept { return is_urgent_; }
  bool IsOneShot() const noexcept { return is_oneshot_; }

  void SetRead(bool is_enable) noexcept { is_read_ = is_enable; }
  void SetWrite(bool is_enable) noexcept { is_write_ = is_enable; }
  void SetError(bool is_enable) noexcept { is_error_ = is_enable; }
  void SetClose(bool is_enable) noexcept { is_close_ = is_enable; }
  void SetEdgeTriggeder(bool is_enable) noexcept { is_et_ = is_enable; }
  void SetUrgetnRead(bool is_enable) noexcept { is_urgent_ = is_enable; }
  void SetOneShot(bool is_enable) noexcept { is_oneshot_ = is_enable; }

 private:
  bool is_read_ = false;
  bool is_write_ = false;
  bool is_error_ = false;
  bool is_close_ = false;
  bool is_et_ = false;  // edge triggered
  bool is_urgent_ = false;
  bool is_oneshot_ = false;
};
}

#endif /* COLLIE_EVENT_H_ */
