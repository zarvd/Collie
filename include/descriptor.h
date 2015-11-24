#ifndef COLLIE_DESCRIPTOR_H_
#define COLLIE_DESCRIPTOR_H_

namespace collie {

/**
 * Descriptor RAII interface
 *
 * when is_init_ == true, the destructor will automatically call
 * this->Close(fd_), which actually call CloseImpl according to is_close_
 */
class Descriptor {
 public:
  Descriptor(const int fd, const bool is_init) noexcept : fd_(fd),
                                                          is_init_(is_init),
                                                          is_close_(false) {}
  Descriptor(const Descriptor &) = delete;
  Descriptor &operator=(const Descriptor &) = delete;
  virtual ~Descriptor() noexcept = 0;

  virtual void Open() = 0;
  void Close() noexcept {
    if (!is_close_ && is_init_) {
      CloseImpl();
      is_close_ = true;
    }
  }

  virtual int fd() const { return fd_; }
  virtual bool is_init() const { return is_init_; }
  bool is_close() const { return is_close_; }

 protected:
  virtual void CloseImpl() noexcept = 0;

  int fd_;
  bool is_init_;

 private:
  bool is_close_;
};
}

#endif /* COLLIE_DESCRIPTOR_H_ */
