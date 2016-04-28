#ifndef COLLIE_SOCKET_H_
#define COLLIE_SOCKET_H_

#include <memory>
#include "util/noncopyable.h"
#include "util/descriptor.h"
#include "exception.h"

namespace collie {

class InetAddress;

class Socket : public NonCopyable, public Descriptor {
 public:
  using Address = std::shared_ptr<InetAddress>;

  Socket() noexcept;
  virtual ~Socket() noexcept = 0;

  int GetDescriptor() const noexcept override { return fd_; }
  void Close() noexcept;
  Address address() const noexcept { return address_; }

 protected:
  int fd_;
  Address address_;
};
}

#endif /* COLLIE_SOCKET_H_ */
