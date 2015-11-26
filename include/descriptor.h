#ifndef COLLIE_DESCRIPTOR_H_
#define COLLIE_DESCRIPTOR_H_

namespace collie {

/**
 * Descriptor interface
 *
 * Pure virtual method:
 * ~Descriptor() noexcept
 * Create()
 * Close()
 *
 * is_init_ is a flag for calling Create() successfully
 * is_close_ is a flag for calling Close() successfully
 *
 * NOTE: For RAII, Derived class's destructor should contains Close() method:
 * Derived::~Derived() {
 * ...
 * Close();
 * ...
 * }
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
