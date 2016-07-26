#include "../../inc/tcp/AsyncTCPServer.h"
#include "../../inc/base/EventThreadPool.h"
#include "../../inc/base/InetAddress.h"
#include "../../inc/base/Logger.h"
#include "../../inc/tcp/AsyncTCPStream.h"
#include "../../inc/tcp/TCPSocket.h"

namespace collie {
namespace tcp {

AsyncTCPServer::AsyncTCPServer() noexcept {}

AsyncTCPServer::~AsyncTCPServer() noexcept {}

AsyncTCPServer& AsyncTCPServer::Listen(const unsigned port,
                                       const String& host) {
  return Listen(InetAddress::GetInetAddress(host, port));
}

AsyncTCPServer& AsyncTCPServer::Listen(
    std::shared_ptr<InetAddress> host_address) {
  if (!event_thread_pool) {
    LOG(ERROR) << "Event Thread Pool is NULL";
    exit(-1);
  }
  auto socket = std::make_shared<TCPSocket>();
  socket->Listen(host_address);
  // socket->SetNonBlocking();
  socket_stream = std::make_shared<AsyncTCPStream>(socket);
  socket_stream->SetReadHander([this](auto) { this->Accept(); });
  EventType event;
  event.SetRead(true);
  event.SetEdgeTriggeder(true);  // set edge triggered
  socket_stream->SetEvent(event);
  event_thread_pool->PushInit(socket_stream);  // push listening socket
  return *this;
}

void AsyncTCPServer::SetEventLoop(
    std::shared_ptr<EventThreadPool> pool) noexcept {
  event_thread_pool = pool;
}

void AsyncTCPServer::Accept() {
  std::shared_ptr<TCPSocket> client;
  client = socket->Accept();
  if (!client) return;
  // client->SetNonBlocking();
  auto client_stream = std::make_shared<AsyncTCPStream>(client);
  event_thread_pool->Push(client_stream);
  req_handler(client_stream);
}
}
}
