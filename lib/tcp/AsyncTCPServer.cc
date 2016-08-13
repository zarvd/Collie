#include "../../inc/tcp/AsyncTCPServer.h"
#include "../../inc/base/Logger.h"

namespace collie {
namespace tcp {

AsyncTCPServer::AsyncTCPServer(std::shared_ptr<EventThreadPool> pool) noexcept
    : event_thread_pool(pool) {}

AsyncTCPServer::~AsyncTCPServer() {}

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

  socket = TCPSocket::Listen(host_address);

  LOG(INFO) << "TCP Server listening " << host_address->ToString();
  return *this;
}

void AsyncTCPServer::Start() {
  if (!event_thread_pool) {
    LOG(ERROR) << "Event Thread Pool is NULL";
    exit(-1);
  }

  while (true) Accept();
}

// Blocking
void AsyncTCPServer::Accept() {
  std::unique_ptr<TCPSocket> client;
  client = socket->Accept();
  if (!client) return;
  // client->SetNonBlocking();
  auto client_stream = std::make_shared<AsyncTCPStream>(std::move(client));
  event_thread_pool->Push(client_stream);
  req_handler(client_stream);
}
}
}
