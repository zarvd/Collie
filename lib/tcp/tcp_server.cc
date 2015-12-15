#include "../../include/event/eventloop.h"
#include "../../include/event/channel.h"
#include "../../include/event/eventloop_threadpool.h"
#include "../../include/inet_address.h"
#include "../../include/tcp/tcp_server.h"
#include "../../include/tcp/tcp_acceptor.h"
#include "../../include/tcp/tcp_socket.h"
#include "../../include/tcp/tcp_iostream.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPServer::TCPServer() : thread_num_(1), port_(0) {}

TCPServer::~TCPServer() {}

void TCPServer::Bind(const std::string& host, const unsigned port) {
  host_ = host;
  port_ = port;
  local_address_ = InetAddress::GetInetAddress(host, port);
}

// Start TCP server
// it creates a `EventloopThreadpool` object, and the thread number
// of the thread pool depends on `this->thread_num_`
// and then, `TCPAcceptor` inserts a base `Channel` into `EventloopThreadpool`
// in the end, `EventloopThreadpool` starts looping
void TCPServer::Start() {
  LOG(INFO) << "TCPServer start in" << local_address_->ip() << ":"
            << local_address_->port();

  acceptor_.reset(new TCPAcceptor(local_address_));
  acceptor_->BindAndListen();
  using namespace std::placeholders;
  // setup acceptor
  acceptor_->set_accept_callback(
      std::bind(&TCPServer::NewConnection, this, _1));
  eventloop_threadpool_.reset(new event::EventLoopThreadPool(thread_num_));
  auto accept_channel = acceptor_->GetBaseChannel();
  eventloop_threadpool_->StartLoop({accept_channel});
}

void TCPServer::NewConnection(std::shared_ptr<TCPSocket> conn_socket) {
  LOG(INFO) << "TCPServer accept fd(" << conn_socket->fd() << ") ip("
            << conn_socket->address()->ip() << ")";

  // new channel
  std::shared_ptr<event::Channel> channel(new event::Channel(conn_socket));
  // NOTE setting up channel in connection
  channel->set_insert_callback([
    on_message_callback = on_message_callback_,
    local_address = local_address_,
    peer_address = conn_socket->address()
  ](std::shared_ptr<event::Channel> channel) {

    CHECK(channel);
    channel->DisableRead();
    channel->DisableWrite();
    // new connection
    auto iostream = std::make_shared<TCPIOStream>(channel);
    g_local_thread_tcp_iostream_set.insert(iostream);
    // store iostream
    on_message_callback(iostream, local_address, peer_address);  // FIXME
    if (channel->IsNoneEvent()) channel->Remove();
  });
  eventloop_threadpool_->PushChannel(channel);

  if (connected_callback_) connected_callback_();
}
}
}
