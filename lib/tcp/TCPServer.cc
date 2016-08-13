#include "../../inc/tcp/TCPServer.h"
#include <sys/socket.h>
#include <unistd.h>
#include "../../inc/base/InetAddress.h"
#include "../../inc/base/Logger.h"
#include "../../inc/tcp/TCPSocket.h"

namespace collie {
namespace tcp {

TCPServer::~TCPServer() {}

TCPServer& TCPServer::Listen(const unsigned port, const String& host) {
  return Listen(InetAddress::GetInetAddress(host, port));
}

TCPServer& TCPServer::Listen(std::shared_ptr<InetAddress> host_address) {
  socket = TCPSocket::Listen(host_address);
  return *this;
}

void TCPServer::Start(const bool is_loop) {
  Accept();
  while (is_loop) Accept();
}

void TCPServer::Accept() {
  std::unique_ptr<TCPSocket> client;
  try {
    client = socket->Accept();
  } catch (std::system_error& e) {
    LOG(ERROR) << e.what();
  }
  TCPStream tcp_stream(std::move(client));
  req_handler(tcp_stream);
}
}
}
