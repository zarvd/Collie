#ifndef COLLIE_DESCRIPTOR_H_
#define COLLIE_DESCRIPTOR_H_

namespace collie {

/**
 * An interface describing the instance is a file or socket descriptor
 */
class Descriptor {
 public:
  Descriptor(const int fd, const bool is_init) noexcept : fd_(fd),
                                                          is_init_(is_init),
                                                          is_close_(false) {}
  Descriptor(const Descriptor &) = delete;
  Descriptor &operator=(const Descriptor &) = delete;
  virtual ~Descriptor() noexcept = 0;

  virtual void Create() = 0;
  virtual void Close() = 0;

  // getter
  virtual int fd() const { return fd_; }
  virtual bool is_init() const { return is_init_; }
  virtual bool is_close() const { return is_close_; }

 protected:
  int fd_;
  bool is_init_;
  bool is_close_;
};
}

#endif /* COLLIE_DESCRIPTOR_H_ */
