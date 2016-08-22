#pragma once

#include <collie/InetAddress.h>
#include <collie/String.h>
#include <collie/tcp/TCPStream.h>
#include <functional>
#include <memory>

namespace collie {

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
