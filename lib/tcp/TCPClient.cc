#include "../../inc/tcp/TCPClient.h"
#include <sys/socket.h>
#include "../../inc/base/InetAddress.h"
#include "../../inc/base/String.h"
#include "../../inc/tcp/TCPSocket.h"
#include "../../inc/tcp/TCPStream.h"

namespace collie {
namespace tcp {

void TCPClient::Connect(const String &host, const unsigned port,
                        const Request &req, const int local_port) {
  auto serv_addr = std::make_shared<InetAddress>(host, port);
  Connect(serv_addr, req, local_port);
}

void TCPClient::Connect(std::shared_ptr<InetAddress> serv_addr,
                        const Request &req, const int local_port) {
  std::unique_ptr<TCPSocket> serv_socket;
  if (local_port != -1)
    serv_socket = TCPSocket::Connect(
        serv_addr, std::make_shared<InetAddress>("0.0.0.0", local_port));
  else
    serv_socket = TCPSocket::Connect(serv_addr);

  TCPStream tcp_stream(std::move(serv_socket));
  req(tcp_stream);
}
}
}
