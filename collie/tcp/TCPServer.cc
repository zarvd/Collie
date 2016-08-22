#include <collie/InetAddress.h>
#include <collie/Logger.h>
#include <collie/tcp/TCPServer.h>
#include <collie/tcp/TCPSocket.h>
#include <sys/socket.h>
#include <unistd.h>

namespace collie {

TCPServer::~TCPServer() {}

TCPServer& TCPServer::Listen(const unsigned port, const std::string& host) {
  return Listen(std::make_shared<InetAddress>(host, port));
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
