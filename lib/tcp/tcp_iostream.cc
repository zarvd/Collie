#include "../../include/tcp/tcp_iostream.h"
#include "../../include/event/channel.h"
#include "../../include/exception.h"
#include "../../include/logging.h"
#include "../../include/descriptor.h"
#include <sys/socket.h>

namespace collie {
namespace tcp {

thread_local std::unordered_set<std::shared_ptr<TCPIOStream>>
    g_local_thread_tcp_iostream_set;

TCPIOStream::TCPIOStream(std::shared_ptr<event::Channel> channel) noexcept
    : channel_(channel),
      is_close_(false),
      default_write_size_(4096),
      default_read_size_(4096) {}

TCPIOStream::~TCPIOStream() noexcept { Close(); }

void TCPIOStream::Close() {
  if (is_close_) return;
  is_close_ = true;
  channel_->Remove();
  g_local_thread_tcp_iostream_set.erase(shared_from_this());
}

// Asynchronously read until we have found the given delimiter
// if this haven't FINISHED and user call another read event, the new callback
// will overwrite the old one
// if max_length is not given, the connection will be closed
// if more than default_read_size_ bytes have been read and the delimiter
// is not found.
void TCPIOStream::ReadUntil(const std::string &delimiter,
                            const ReadCallback &callback,
                            const int max_length) {
  REQUIRE(channel_ && !is_close_);

  const unsigned content_length =
      max_length == -1 ? default_read_size_ : max_length;

  using namespace std::placeholders;
  channel_->set_read_callback(std::bind(&TCPIOStream::HandleReadUntil, this,
                                        delimiter, callback, content_length,
                                        _1));
  channel_->EnableRead();
  read_buffer_.clear();
}

void TCPIOStream::HandleReadUntil(const std::string &delimiter,
                                  const ReadCallback &callback,
                                  const unsigned max_length,
                                  std::shared_ptr<event::Channel> channel) {
  const auto fd = channel->descriptor()->fd();
  char content[max_length];
  const ssize_t recv_size =
      ::recv(fd, content, max_length - read_buffer_.length(), MSG_DONTWAIT);
  if (recv_size == -1) {
    // get error
    Log(WARN) << "TCPIOStream read error: " << GetError();
    Close();
  } else if (recv_size == 0) {
    // get EOF and cannot find delimiter
    // close
    Close();
  } else {
    // get regular bytes
    // find delimiter whether in recved string, then call callback
    const std::string content_string = content;
    auto found = content_string.find(delimiter);
    if (found != std::string::npos) {
      // delimiter not found
      channel->DisableRead();  // disable read before callback in case of
                               // calling another read event in callback
      const auto complete_content =
          read_buffer_ + content_string.substr(0, found + delimiter.length());
      if (callback) callback(complete_content, shared_from_this());
      read_buffer_.clear();
    } else if (read_buffer_.length() + recv_size >= max_length) {
      // read comes to max length, we should close iostream
      Close();
    } else {
      read_buffer_ += content_string;  // append recved content to buffer
    }
  }
  if (channel->IsNoneEvent()) Close();
}

// Asynchronously read until getting EOF
void TCPIOStream::ReadUntilClose(const ReadCallback &callback,
                                 const int max_length) {
  REQUIRE(channel_ && !is_close_);

  const unsigned content_length =
      max_length == -1 ? default_read_size_ : max_length;

  using namespace std::placeholders;
  channel_->set_read_callback(std::bind(&TCPIOStream::HandleReadUntilClose,
                                        this, callback, content_length, _1));
  channel_->EnableRead();
  read_buffer_.clear();
}

void TCPIOStream::HandleReadUntilClose(
    const ReadCallback &callback, const unsigned max_length,
    std::shared_ptr<event::Channel> channel) {
  const auto fd = channel->descriptor()->fd();
  char content[max_length];
  const ssize_t recv_size =
      ::recv(fd, content, max_length - read_buffer_.length(), MSG_DONTWAIT);
  if (recv_size == -1) {
    // get error
    Log(WARN) << "TCPIOStream read error: " << GetError();
    Close();
  } else if (recv_size == 0) {
    // get EOF
    channel->DisableRead();
    if (callback) callback(std::move(read_buffer_), shared_from_this());
    read_buffer_.clear();
  } else if (recv_size + read_buffer_.length() >= max_length) {
    // read bytes over max length, close
    Close();
  } else {
    // get regular bytes
    read_buffer_ += content;
  }
  if (channel->IsNoneEvent()) Close();
}

void TCPIOStream::Write(const std::string &data, const int package_length,
                        const WriteCallback &callback) {
  REQUIRE(channel_ && !is_close_);

  write_buffer_ = std::move(data);

  const unsigned p_len =
      package_length == -1 ? default_write_size_ : package_length;

  using namespace std::placeholders;
  channel_->set_write_callback(
      std::bind(&TCPIOStream::HandleWrite, this, callback, p_len, _1));
  channel_->EnableWrite();
}

void TCPIOStream::HandleWrite(const WriteCallback &callback,
                              const unsigned package_length,
                              std::shared_ptr<event::Channel> channel) {
  if (write_buffer_.empty()) {
    // write complete
    channel->DisableWrite();
    if (callback) callback(shared_from_this());
  } else {
    // keep writing
    const auto fd = channel->descriptor()->fd();
    const unsigned p_len = package_length > write_buffer_.length()
                               ? write_buffer_.length()
                               : package_length;
    char buffer[p_len];
    std::strncpy(buffer, write_buffer_.c_str(), sizeof(buffer));
    int ret = ::send(fd, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (ret == -1) {
      // get error
      Log(WARN) << "TCPIOStream write error: " << GetError();
      Close();
    } else {
      if (static_cast<unsigned>(ret) >= write_buffer_.length()) {
        // TODO log warn
        write_buffer_.clear();
      } else {
        write_buffer_.substr(ret);
      }
      if (write_buffer_.empty()) {
        channel->DisableWrite();
        if (callback) callback(shared_from_this());
      }
    }
  }

  if (channel->IsNoneEvent()) Close();
}
}
}
