#include "../../include/tcp/tcp_iostream.h"
#include "../../include/event/channel.h"
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

void TCPIOStream::ReadUntil(const std::string &delimiter,
                            const ReadCallback &callback,
                            const int max_length) {
  CHECK(channel_ && !is_close_);

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
    // Gets error
    LOG(WARNING) << "TCPIOStream read error: " << GetSystemError();
    Close();
  } else if (recv_size == 0) {
    // Gets EOF and cannot find delimiter
    Close();
  } else {
    // Gets regular bytes
    // Finds delimiter if it is in receiving string, then call `callback`
    const std::string content_string = content;
    auto found = content_string.find(delimiter);
    if (found != std::string::npos) {
      // Delimiter not found
      channel->DisableRead();  // disable read before callback in case of
                               // calling another read event in callback
      const auto complete_content =
          read_buffer_ + content_string.substr(0, found + delimiter.length());
      if (callback) callback(complete_content, shared_from_this());
      read_buffer_.clear();
    } else if (read_buffer_.length() + recv_size >= max_length) {
      // Comes to max length, closes the instance
      Close();
    } else {
      read_buffer_ += content_string;  // append recved content to buffer
    }
  }
  if (channel->IsNoneEvent()) Close();
}

void TCPIOStream::ReadUntilClose(const ReadCallback &callback,
                                 const int max_length) {
  CHECK(channel_ && !is_close_);

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
    // Gets error
    LOG(WARNING) << "TCPIOStream read error: " << GetSystemError();
    Close();
  } else if (recv_size == 0) {
    // Gets EOF
    channel->DisableRead();
    if (callback) callback(std::move(read_buffer_), shared_from_this());
    read_buffer_.clear();
  } else if (recv_size + read_buffer_.length() >= max_length) {
    // Comes to max length, closes the instance
    Close();
  } else {
    // Get regular bytes
    read_buffer_ += content;
  }
  if (channel->IsNoneEvent()) Close();
}

void TCPIOStream::Write(const std::string &data, const int package_length,
                        const WriteCallback &callback) {
  CHECK(channel_ && !is_close_);

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
    // Writes complete
    channel->DisableWrite();
    if (callback) callback(shared_from_this());
  } else {
    // Keeps writing
    const auto fd = channel->descriptor()->fd();
    const unsigned p_len = package_length > write_buffer_.length()
                               ? write_buffer_.length()
                               : package_length;
    char buffer[p_len];
    std::strncpy(buffer, write_buffer_.c_str(), sizeof(buffer));
    int ret = ::send(fd, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (ret == -1) {
      // Gets error
      LOG(WARNING) << "TCPIOStream write error: " << GetSystemError();
      Close();
    } else {
      if (static_cast<unsigned>(ret) >= write_buffer_.length()) {
        // Sends all buffer
        write_buffer_.clear();
      } else {
        write_buffer_.substr(ret);
      }
      if (write_buffer_.empty()) {
        // Writes complete
        channel->DisableWrite();
        if (callback) callback(shared_from_this());
      }
    }
  }

  if (channel->IsNoneEvent()) Close();
}
}
}
