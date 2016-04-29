#ifndef COLLIE_TCP_CLIENT_H_
#define COLLIE_TCP_CLIENT_H_

#include <memory>
#include <functional>
#include "exception.h"

namespace collie {

class InetAddress;
class TcpStream;

class TcpClient {
 public:
  using Address = std::shared_ptr<InetAddress>;
  using Host = std::string;
  using Port = unsigned;
  using Request = std::function<void(TcpStream&)>;

  TcpClient() = delete;

  static void Connect(const Host& host, const Port& port,
                      const Request& req) throw(TcpException);
  static void Connect(Address serv_addr,
                      const Request& req) throw(TcpException);
};
}

#endif /* COLLIE_TCP_CLIENT_H_ */
