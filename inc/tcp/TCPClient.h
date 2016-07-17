#ifndef COLLIE_TCP_CLIENT_H_
#define COLLIE_TCP_CLIENT_H_

#include <functional>
#include <memory>

namespace collie {
class InetAddress;
class String;

namespace tcp {

class TCPStream;

class TCPClient {
 public:
  using Request = std::function<void(TCPStream&)>;

  TCPClient() = delete;

  static void Connect(const String& host, const unsigned& port,
                      const Request& req);
  static void Connect(std::shared_ptr<InetAddress> serv_addr,
                      const Request& req);
};
}
}

#endif /* COLLIE_TCP_CLIENT_H_ */
