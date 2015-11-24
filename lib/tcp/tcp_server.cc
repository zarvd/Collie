#include "../../include/event/eventloop.h"
#include "../../include/event/channel.h"
#include "../../include/event/eventloop_threadpool.h"
#include "../../include/inet_address.h"
#include "../../include/tcp/tcp_server.h"
#include "../../include/tcp/tcp_acceptor.h"
#include "../../include/tcp/tcp_socket.h"
#include "../../include/tcp/tcp_connection.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPServer::TCPServer() : thread_num_(1), port_(0) {
  Log(TRACE) << "TCPServer constructing";
}

TCPServer::~TCPServer() { Log(TRACE) << "TCPServer destructing"; }

void TCPServer::Bind(const std::string& host, const unsigned port) {
  host_ = host;
  port_ = port;
  local_address_ = InetAddress::GetInetAddress(host, port);
}

void TCPServer::Start() {
  Log(INFO) << "TCPServer start";

  acceptor_.reset(new TCPAcceptor(local_address_));
  using namespace std::placeholders;
  // setup acceptor
  acceptor_->set_accept_callback(
      std::bind(&TCPServer::NewConnection, this, _1));
  eventloop_threadpool_.reset(new event::EventLoopThreadPool(thread_num_));
  auto accept_channel = acceptor_->GetBaseChannel();
  eventloop_threadpool_->StartLoop({accept_channel});
}

void TCPServer::NewConnection(std::shared_ptr<TCPSocket> conn_socket) {
  Log(INFO) << "TCPServer accept fd(" << conn_socket->fd() << ") ip("
            << conn_socket->address()->ip() << ")";

  // new channel
  std::shared_ptr<event::Channel> channel(new event::Channel(conn_socket));
  // NOTE setting up channel in connection
  channel->set_after_set_loop_callback([
    on_message_callback = on_message_callback_,
    local_address = local_address_
  ](std::shared_ptr<event::Channel> channel) {

    // new connection
    auto connection = std::make_shared<TCPConnection>(channel, local_address);
    connection->set_message_callback(on_message_callback);
    // store this connection in server
    g_local_thread_connections.insert(connection);

    connection->set_terminate_callback([](std::shared_ptr<TCPConnection> conn) {
      // remove this connection from server
      Log(INFO) << "Connection close";
      g_local_thread_connections.erase(conn);
    });
  });
  eventloop_threadpool_->PushChannel(channel);

  if (connected_callback_) connected_callback_();
}
}
}
