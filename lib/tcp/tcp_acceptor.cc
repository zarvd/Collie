#include "../../include/tcp/tcp_acceptor.h"
#include "../../include/tcp/tcp_socket.h"
#include "../../include/event/eventloop.h"
#include "../../include/event/channel.h"
#include "../../include/inet_address.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPAcceptor::TCPAcceptor(std::shared_ptr<InetAddress> addr) noexcept
    : thread_num_(1),
      local_address_(addr),
      tcp_socket_(new TCPSocket(local_address_)) {
  Log(TRACE) << "TCPAcceptor constructing";
}

TCPAcceptor::~TCPAcceptor() noexcept {
  Log(TRACE) << "TCPAcceptor destructing";
}

void TCPAcceptor::BindAndListen() const { tcp_socket_->BindAndListen(); }

void TCPAcceptor::set_thread_num(const size_t thread_num) {
  thread_num_ = thread_num;
}

std::shared_ptr<event::Channel> TCPAcceptor::GetBaseChannel() {
  REQUIRE_(tcp_socket_, "TCP Socket is NULL");

  // create channel
  auto channel = std::make_shared<event::Channel>(tcp_socket_);
  // for multi threads it doesn't own the socket fd
  // set callback after setting event loop
  channel->set_after_set_loop_callback(
      [this](std::shared_ptr<event::Channel> channel) {
        Log(TRACE) << "TCPAcceptor channel is setting up";
        channel->EnableRead();
        channel->set_read_callback(std::bind(&TCPAcceptor::HandleRead, this));
        channel->set_error_callback(std::bind(&TCPAcceptor::HandleError, this));
        channel->EnableOneShot();  // NOTE One shot, channel needs to update
                                   // after every accepting
        channel->set_update_after_activate(true);
      });
  return channel;
}

void TCPAcceptor::HandleRead() {
  // should be thread-safe
  auto connSocket = tcp_socket_->Accept();
  if (connSocket->state() == TCPSocket::State::IllegalAccept) {
    HandleError();
  } else {
    REQUIRE(accept_callback_);
    accept_callback_(connSocket);
  }
}

void TCPAcceptor::HandleError() {
  // TODO
  Log(TRACE) << "TCPAcceptor handle error";
}
}
}
