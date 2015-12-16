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

// TCP socket
// Creates a tcp socket which is a listening socket(server), a connecting
// socket(client) or an accepting socket(connection).
//
// The `state()` returns `State::Accept` when instance is an accepting socket.
// An accepting socket is created by a listening socket.
//
// A socket, whose state is `State::Accept` or `State::Connect`, is able to be
// wrote or read. And the socket with other states is not able to do so.
//
// Usages:
//   auto tcp_socket = std::make_shared<TCPSocket>(local_address);
//   tcp_socket->BindAndListen();
//   auto channel = std::make_shared<Channel>(tcp_socket);
class TCPSocket : public Descriptor,
                  public std::enable_shared_from_this<TCPSocket> {
 public:
  enum class State {
    Init = 1,
    Socket = 2,
    Bind = 3,
    Listen = 4,
    Accept = 5,         // Accept succeeds
    IllegalAccept = 6,  // Accept fails
    Connect = 7,
    Close = 8
  };

  explicit TCPSocket(std::shared_ptr<InetAddress> localAddr) noexcept;
  TCPSocket(const TCPSocket &) = delete;
  TCPSocket &operator=(const TCPSocket &) = delete;
  ~TCPSocket() noexcept override;

  // Sets socket fd option `TCP_NODELAY`.
  // Throws if `state_` is `State::Init`, `State::IllegalAccept` or
  // `State::Close`.
  // If the operation fails, it doesn't throw but logs WARNING message.
  void SetNoDelay() const;
  // The interfaces of `Descriptor` which actually calls `CreateImpl()` and
  // `CloseImpl()`.
  void Create() noexcept override;
  void Close() noexcept override;

  // Returns current state.
  State state() const { return state_; }
  // Returns local address.
  std::shared_ptr<InetAddress> address() const { return address_; }

  // TCP client connects to `peer_address`.
  // Returns false when the operation fails or ture when the operation succeeds.
  bool Connect(std::shared_ptr<InetAddress> peer_address);
  // TCP server binds and listens `address_`.
  // Returns false when the operation fails or ture when the operation succeeds.
  bool BindAndListen();
  // Returns an accepting socket whose state is `State::Accept` or
  // `State::IllegalAccept`.
  std::shared_ptr<TCPSocket> Accept(bool blocking = false);

 private:
  // Constructs an accepting socket whose state is `State::Accept`.
  TCPSocket(const int fd, std::shared_ptr<InetAddress> addr) noexcept;
  // Constructs an accepting socket whose state is `State::IllegalAccept`.
  TCPSocket() noexcept;

  // Returns an accepting socket whose state is `State::Accept`.
  std::shared_ptr<TCPSocket> GetAcceptSocket(const int fd,
                                             std::shared_ptr<InetAddress>);
  // Returns an accepting socket whose state is `State::IllegalAccept`.
  std::shared_ptr<TCPSocket> GetIllegalAcceptSocket() noexcept;

  void CreateImpl() noexcept;
  void CloseImpl() noexcept;

  // Operations of socket.
  bool ListenV4();
  std::shared_ptr<TCPSocket> AcceptV4(bool blocking);
  bool ConnectV4(std::shared_ptr<InetAddress> servAddr);

  State state_;
  std::shared_ptr<InetAddress> address_;
};
}
}

#endif /* COLLIE_TCP_TCPSOCKET_H_ */
