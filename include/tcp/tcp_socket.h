#ifndef COLLIE_TCP_TCPSOCKET_H_
#define COLLIE_TCP_TCPSOCKET_H_

#include <memory>
#include "../descriptor.h"

namespace collie {

class InetAddress;

namespace tcp {

// TCP socket
class TCPSocket : public Descriptor,
                  public std::enable_shared_from_this<TCPSocket> {
 public:
  enum class State { Error, Unconnected, Bound, Listening, Connected, Closed };

  TCPSocket();
  TCPSocket(const TCPSocket &) = delete;
  TCPSocket &operator=(const TCPSocket &) = delete;
  ~TCPSocket() override;

  bool SetNoDelay(const bool) const;
  void Create() override;
  void Close() override;

  State state() const { return state_; }
  std::shared_ptr<InetAddress> local_address() const { return local_address_; }
  std::shared_ptr<InetAddress> peer_address() const { return peer_address_; }

  bool ConnectTo(std::shared_ptr<InetAddress> peer_address);
  bool Bind(std::shared_ptr<InetAddress> local_address);
  std::shared_ptr<TCPSocket> Accept(bool blocking = false);
  bool Write(const std::string &data);
  std::string Read(const ssize_t);
  static std::shared_ptr<TCPSocket> GetErrorSocket();

 private:
  std::shared_ptr<TCPSocket> GetConnectedSocket(
      const int fd, std::shared_ptr<InetAddress> peer_address);

  void CreateImpl();
  void CloseImpl();

  // Operations of socket.
  std::shared_ptr<TCPSocket> AcceptV4(bool blocking);
  bool ConnectToV4(std::shared_ptr<InetAddress> peer_address);

  State state_;
  std::shared_ptr<InetAddress> local_address_;
  std::shared_ptr<InetAddress> peer_address_;
};
}
}

#endif /* COLLIE_TCP_TCPSOCKET_H_ */
