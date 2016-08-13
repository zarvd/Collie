#include "../../inc/tcp/TCPClient.h"
#include <sys/socket.h>
#include "../../inc/base/InetAddress.h"
#include "../../inc/base/String.h"
#include "../../inc/tcp/TCPSocket.h"
#include "../../inc/tcp/TCPStream.h"

namespace collie {
namespace tcp {

void TCPClient::Connect(const String &host, const unsigned &port,
                        const Request &req) {
  auto serv_addr = InetAddress::GetInetAddress(host, port);
  Connect(serv_addr, req);
}

void TCPClient::Connect(std::shared_ptr<InetAddress> serv_addr,
                        const Request &req) {
  auto serv_socket = TCPSocket::Connect(serv_addr);

  TCPStream tcp_stream(std::move(serv_socket));
  req(tcp_stream);
}
}
}
