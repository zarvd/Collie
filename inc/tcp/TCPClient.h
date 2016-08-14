#ifndef COLLIE_TCP_CLIENT_H_
#define COLLIE_TCP_CLIENT_H_

#include <functional>
#include <memory>
#include "../base/InetAddress.h"
#include "../base/String.h"

namespace collie {
namespace tcp {

class TCPStream;

class TCPClient {
 public:
  using Request = std::function<void(TCPStream&)>;

  TCPClient() = delete;

  static void Connect(const std::string& host, const unsigned port,
                      const Request& req, const int local_port = -1);
  static void Connect(std::shared_ptr<InetAddress> serv_addr,
                      const Request& req, const int local_port = -1);
};
}
}

#endif /* COLLIE_TCP_CLIENT_H_ */
