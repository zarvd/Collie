#include "../../include/event/eventloop.h"
#include "../../include/event/channel.h"
#include "../../include/event/eventloop_threadpool.h"
#include "../../include/inet_address.h"
#include "../../include/tcp/tcp_server.h"
#include "../../include/tcp/tcp_socket.h"
#include "../../include/tcp/tcp_iostream.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPServer::TCPServer() : thread_num_(1), port_(0) {
  listen_socket_ = std::make_shared<TCPSocket>();
}

TCPServer::~TCPServer() {}

void TCPServer::Bind(const std::string& host, const unsigned port) {
  host_ = host;
  port_ = port;
  local_address_ = InetAddress::GetInetAddress(host, port);
  listen_socket_->Bind(local_address_);
}

// Start TCP server
// it creates a `EventloopThreadpool` object, and the thread number
// of the thread pool depends on `this->thread_num_`
// and then, `TCPAcceptor` inserts a base `Channel` into `EventloopThreadpool`
// in the end, `EventloopThreadpool` starts looping
void TCPServer::Start() {
  LOG(INFO) << "TCPServer start in" << local_address_->ip() << ":"
            << local_address_->port();
  using namespace std::placeholders;
  // setup acceptor
  auto channel = std::make_shared<event::Channel>(listen_socket_);
  auto eventloop_threadpool =
      event::EventLoopThreadPool::GetEventLoopThreadPool(4);
  channel->set_insert_callback([this](auto channel) {
    channel->EnableRead();
    channel->set_read_callback(std::bind(&TCPServer::HandleAccept, this));
    channel->set_error_callback(std::bind(&TCPServer::HandleError, this));
    channel->EnableOneShot();  // NOTE One shot, channel needs to update
    // after every accepting
    channel->set_update_after_activate(true);
  });
  eventloop_threadpool->StartLoop({channel});
}

void TCPServer::HandleAccept() {
  auto connected_socket = listen_socket_->Accept();
  if (connected_socket) {
    HandleConnection(connected_socket);
  }
}

void TCPServer::HandleError() {}

void TCPServer::HandleConnection(std::shared_ptr<TCPSocket> conn_socket) {
  LOG(INFO) << "TCPServer accept fd(" << conn_socket->fd() << ") ip("
            << conn_socket->peer_address()->ip() << ")";

  // new channel
  std::shared_ptr<event::Channel> channel(new event::Channel(conn_socket));

  channel->set_insert_callback([on_message_callback = on_message_callback_](
      std::shared_ptr<event::Channel> channel) {

    CHECK(channel);
    channel->DisableRead();
    channel->DisableWrite();
    // new connection
    auto iostream = std::make_shared<TCPIOStream>(channel);
    g_local_thread_tcp_iostream_set.insert(iostream);
    // store iostream
    on_message_callback(iostream);
    if (channel->IsNoneEvent()) channel->Remove();
  });
  event::EventLoopThreadPool::PushChannel(channel);

  if (connected_callback_) connected_callback_();
}
}
}
