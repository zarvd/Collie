#ifndef COLLIE_TCP_TCPSOCKET_H_
#define COLLIE_TCP_TCPSOCKET_H_

#include <memory>
#include "../descriptor.h"

namespace collie {

class InetAddress;

namespace utils {
class File;
}

namespace tcp {

class TCPSocket : public Descriptor,
                  public std::enable_shared_from_this<TCPSocket> {
 public:
  enum class State {
    Init = 1,
    Socket = 2,
    Bind = 3,
    Listen = 4,
    Accept = 5,
    IllegalAccept = 6,
    Connect = 7,
    Close = 8
  };
  // Accept: which is the connection socket that accept() returned
  // IllegalAccept: which is the ILLEGAL connection socket that accept()
  // returned

  explicit TCPSocket(std::shared_ptr<InetAddress> localAddr) noexcept;
  TCPSocket(const TCPSocket &) = delete;
  TCPSocket &operator=(const TCPSocket &) = delete;
  ~TCPSocket() noexcept override;

  void SetNoDelay() const;
  void Create() noexcept override;
  void Close() noexcept override;

  // Descriptor
  State state() const { return state_; }
  std::shared_ptr<InetAddress> address() const { return address_; }

  bool Connect(std::shared_ptr<InetAddress>);
  bool BindAndListen();
  std::shared_ptr<TCPSocket> Accept(bool blocking = false);

 private:
  // construct Accept connection socket
  TCPSocket(const int fd, std::shared_ptr<InetAddress> addr) noexcept;
  // construct illegal socket
  TCPSocket() noexcept;

  std::shared_ptr<TCPSocket> GetAcceptSocket(const int fd,
                                             std::shared_ptr<InetAddress>);
  std::shared_ptr<TCPSocket> GetIllegalAcceptSocket() noexcept;

  void CreateImpl() noexcept;
  void CloseImpl() noexcept;

  bool ListenV4();
  std::shared_ptr<TCPSocket> AcceptV4(bool blocking);
  bool ConnectV4(std::shared_ptr<InetAddress> servAddr);

  State state_;
  std::shared_ptr<InetAddress> address_;
};
}
}

#endif /* COLLIE_TCP_TCPSOCKET_H_ */
