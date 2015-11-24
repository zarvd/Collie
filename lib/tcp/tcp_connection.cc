#include "../../include/socket.h"
#include "../../include/inet_address.h"
#include "../../include/event/eventloop.h"
#include "../../include/event/channel.h"
#include "../../include/tcp/tcp_socket.h"
#include "../../include/tcp/tcp_connection.h"
#include "../../include/utils/file.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

thread_local std::unordered_set<std::shared_ptr<TCPConnection>>
    g_local_thread_connections;

TCPConnection::TCPConnection(std::shared_ptr<event::Channel> channel,
                             std::shared_ptr<InetAddress> local_address)
    : connected_(true),
      terminate_(false),
      channel_(channel),
      local_address_(local_address) {
  Log(TRACE) << "TCP Connection is constructing";
  REQUIRE(channel);
  // set channel callback and enable reading
  channel->set_read_callback(std::bind(&TCPConnection::HandleRead, this));
  channel->set_write_callback(std::bind(&TCPConnection::HandleWrite, this));
  channel->set_close_callback(std::bind(&TCPConnection::HandleClose, this));
  channel->set_error_callback(std::bind(&TCPConnection::HandleError, this));
  channel->EnableRead();
  channel->EnableWrite();
  auto fd = channel_->descriptor();
  auto socket = std::static_pointer_cast<TCPSocket>(fd);
  remote_address_ = socket->address();
}

TCPConnection::~TCPConnection() {
  Log(TRACE) << "TCP Connection is destructing";
}

void TCPConnection::Disconnect() {
  Log(DEBUG) << "TCP Connection is trying to disconnect";
  connected_ = false;
  if (outputBuffer_.empty()) Terminate();
}

void TCPConnection::Terminate() {
  if (terminate_) {
    Log(DEBUG) << "TCP Connection was already shut down";
    return;
  }
  terminate_ = true;
  Log(TRACE) << "TCP Connection is shutting down";
  channel_->Remove();
  if (terminate_callback_) terminate_callback_(shared_from_this());
}

std::string TCPConnection::RecvAll() {
  // FIXME
  std::string buffer;
  std::swap(buffer, inputBuffer_);
  return buffer;
}

void TCPConnection::Send(const std::string& buffer) {
  // FIXME
  outputBuffer_ += buffer;
  if (!channel_->IsWrite()) channel_->EnableWrite();
}

void TCPConnection::SendFile(const std::string& pathname) {
  REQUIRE(channel_);
  utils::File file(pathname, utils::File::Mode::Read);
  if (file.is_existed() && file.IsFile()) {
    socket::SendFile(channel_->descriptor(), file);
  } else {
    // XXX
    Log(WARN) << pathname << " not found";
  }
}

void TCPConnection::SendFile(const utils::File& file) {
  if (file.is_existed() && file.IsFile()) {
    socket::SendFile(channel_->descriptor(), file);
  } else {
    // XXX
    Log(WARN) << file.kPathName << " not found";
  }
}

void TCPConnection::RecvFile(const std::string& pathname,
                             const size_t recv_size) {
  REQUIRE(channel_);
  utils::File file(pathname, utils::File::Mode::Write);
  socket::RecvFile(channel_->descriptor(), file, recv_size);
}

void TCPConnection::HandleRead() {
  std::string content;
  // Non blocking receiving
  const auto size = socket::Recv(channel_->descriptor(), content, MSG_DONTWAIT);
  if (size > 0) {
    inputBuffer_ += content;
    message_callback_(shared_from_this());
    channel_->DisableRead();
    if (!outputBuffer_.empty()) {
      if (!channel_->IsWrite()) channel_->EnableWrite();
    } else if (!connected_) {
      Terminate();
    }
  } else if (size == 0) {
    HandleClose();
  } else {
    HandleError();
  }
}

void TCPConnection::HandleWrite() {
  // XXXX
  if (!outputBuffer_.empty()) {
    const ssize_t size =
        socket::Send(channel_->descriptor(), outputBuffer_, MSG_DONTWAIT);
    // const ssize_t bufferSize = outputBuffer.length();
    // if(bufferSize == size - 1) {
    //     outputBuffer.clear();
    // } else if(size > 0) {
    //     outputBuffer = outputBuffer.substr(size - 1);
    // }
    if (size > 0) outputBuffer_.clear();
  }
  channel_->DisableWrite();
  channel_->EnableRead();
  if (!connected_) Terminate();
}

void TCPConnection::HandleClose() { Disconnect(); }

void TCPConnection::HandleError() { Terminate(); }
}
}
