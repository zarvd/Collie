#include "../inc/async_tcp_stream.h"
#include "../inc/async_tcp_server.h"
#include "../inc/tcp_socket.h"
#include "../inc/inet_address.h"
#include "../inc/logger.h"
#include "../inc/poll/event_thread_pool.h"

namespace collie {

AsyncTcpServer::AsyncTcpServer() noexcept {}

AsyncTcpServer::~AsyncTcpServer() noexcept {}

AsyncTcpServer& AsyncTcpServer::Listen(const Port port,
                                       const Host& host) throw(TcpException) {
  return Listen(InetAddress::GetInetAddress(host, port));
}

AsyncTcpServer& AsyncTcpServer::Listen(Address host_address) throw(
    TcpException) {
  try {
    socket_fd_ = std::make_shared<TcpSocket>();
    socket_fd_->Listen(host_address);
    socket_fd_->SetNonBlocking();
  } catch (TcpException& e) {
    LOG(ERROR) << e.what();
  }
  return *this;
}

void AsyncTcpServer::SetLoop(std::shared_ptr<EventThreadPool> pool) noexcept {
  event_thread_pool_ = pool;
  socket_stream_ = std::make_shared<AsyncTcpStream>(socket_fd_);
  socket_stream_->set_read_hander([this](auto) { this->Accept(); });
  Event event;
  event.SetRead(true);
  event.SetEdgeTriggeder(true);  // set edge triggered
  socket_stream_->set_event(event);
  event_thread_pool_->PushInit(socket_stream_);  // push listening socket
}

void AsyncTcpServer::Accept() throw(TcpException) {
  std::shared_ptr<TcpSocket> client;
  try {
    client = socket_fd_->Accept();
    if (!client) return;
    client->SetNonBlocking();
    auto client_stream = std::make_shared<AsyncTcpStream>(client);
    event_thread_pool_->Push(client_stream);
    req_handler_(client_stream);
  } catch (TcpException& e) {
    LOG(ERROR) << e.what();
  }
}
}
