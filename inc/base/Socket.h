#ifndef COLLIE_BASE_SOCKET_H_
#define COLLIE_BASE_SOCKET_H_

#include <memory>
#include "../util/NonCopyable.h"

namespace collie {

class InetAddress;

class Socket : public util::NonCopyable {
 public:
  using AddressPtr = std::shared_ptr<InetAddress>;

  constexpr Socket() noexcept : fd(-1) {}
  virtual ~Socket() noexcept = 0;

  void SetNonBlocking() noexcept;
  void Close() noexcept;
  AddressPtr Address() const noexcept { return address; }

 protected:
  int fd;
  AddressPtr address;
};
}

#endif /* COLLIE_BASE_SOCKET_H_ */
