#ifndef COLLIE_TCP_SOCKET_H_
#define COLLIE_TCP_SOCKET_H_

#include "socket.h"

namespace collie {

class TcpSocket : public Socket {
 public:
  TcpSocket() noexcept;
  ~TcpSocket() noexcept override;

  void Listen(Address) throw(TcpException);
  void Connect(Address) throw(TcpException);
  std::shared_ptr<TcpSocket> Accept() throw(TcpException);
};
}

#endif /* COLLIE_TCP_SOCKET_H_ */
