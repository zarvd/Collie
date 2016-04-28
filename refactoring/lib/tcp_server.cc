#include "../inc/inet_address.h"
#include "../inc/logger.h"
#include "../inc/tcp_server.h"
#include "../inc/tcp_stream.h"
#include "../inc/tcp_socket.h"
#include <sys/socket.h>
#include <unistd.h>

namespace collie {

TcpServer::TcpServer() noexcept : req_handler_(nullptr) {}
TcpServer::~TcpServer() noexcept {}

TcpServer& TcpServer::Listen(const Port port,
                             const Host& host) throw(TcpException) {
  return Listen(InetAddress::GetInetAddress(host, port));
}

TcpServer& TcpServer::Listen(Address host_address) throw(TcpException) {
  try {
    socket_fd_ = std::make_shared<TcpSocket>();
    socket_fd_->Listen(host_address);
  } catch (TcpException& e) {
    LOG(ERROR) << e.what();
  }

  return *this;
}

void TcpServer::Start(const bool is_loop) throw(TcpException) {
  Accept();
  while (is_loop) {
    Accept();
  }
}

void TcpServer::Accept() throw(TcpException) {
  std::shared_ptr<TcpSocket> client;
  try {
    client = socket_fd_->Accept();
  } catch (TcpException& e) {
    LOG(ERROR) << e.what();
  }
  TcpStream tcp_stream(client);
  req_handler_(tcp_stream);
}
}
