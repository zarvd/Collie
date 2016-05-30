#ifndef COLLIE_SOCKET_H_
#define COLLIE_SOCKET_H_

#include <memory>
#include "exception.h"
#include "util/descriptor.h"
#include "util/noncopyable.h"

namespace collie {

class InetAddress;

class Socket : public util::NonCopyable, public Descriptor {
 public:
  using Address = std::shared_ptr<InetAddress>;

  Socket() noexcept;
  virtual ~Socket() noexcept = 0;

  void SetNonBlocking() noexcept;
  int GetDescriptor() const noexcept override { return fd; }
  void Close() noexcept;
  Address GetAddress() const noexcept { return address; }

 protected:
  int fd;
  Address address;
};
}

#endif /* COLLIE_SOCKET_H_ */
