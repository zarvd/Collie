#include "../../inc/tcp/AsyncTCPStream.h"
#include <sys/socket.h>
#include <cstring>
#include "../../inc/base/EventPool.h"

namespace collie {
namespace tcp {

AsyncTCPStream::AsyncTCPStream(std::unique_ptr<TCPSocket> socket) noexcept
    : socket(std::move(socket)),
      read_size(3000) {
  write_handler = [](auto) { LOG(WARN) << "No write handler"; };
  read_handler = [](auto) { LOG(WARN) << "No write handler"; };
  error_handler = [](auto) { LOG(WARN) << "No error handler"; };
  close_handler = [](auto) { LOG(WARN) << "No close handler"; };
}

AsyncTCPStream::~AsyncTCPStream() {}

int AsyncTCPStream::Descriptor() const noexcept { return socket->Descriptor(); }

std::shared_ptr<const InetAddress> AsyncTCPStream::LocalAddress() const
    noexcept {
  return socket->LocalAddress();
}
std::shared_ptr<const InetAddress> AsyncTCPStream::PeerAddress() const
    noexcept {
  return socket->PeerAddress();
}

void AsyncTCPStream::Write(const std::string& buf,
                           const AsyncCallback& callback) {
  if (!event_pool) throw std::runtime_error("Event pool is not available");

  write_handler = [buf, callback](auto stream) {
    int ret =
        ::send(stream->Descriptor(), buf.c_str(), buf.length(), MSG_DONTWAIT);
    if (ret == -1) {
      throw std::runtime_error(::strerror(errno));
    } else if ((SizeType)ret < buf.length()) {
      // Some bytes are not sent, resend the left
      stream->Write(buf.substr(ret), callback);
      return;
    }
    stream->event.SetWrite(false);
    stream->event_pool->Update(stream);
    if (!callback) {
      stream->Abort();
    } else {
      callback(stream);
      if (!stream->event.IsRead() && !stream->event.IsWrite()) {
        stream->Abort();
      }
    }
  };

  event.SetWrite(true);

  event_pool->Update(shared_from_this());
}

void AsyncTCPStream::Read(const AsyncCallback& callback) {
  if (!event_pool) throw std::runtime_error("Event pool is not available");

  read_handler = [callback](auto stream) {
    char buffer[stream->read_size];
    const int ret =
        ::recv(stream->Descriptor(), buffer, stream->read_size, MSG_DONTWAIT);

    if (ret == -1) {
      throw std::runtime_error(::strerror(errno));
    } else if (ret == 0) {
      // close
      stream->Abort();
      return;
    }
    // Disables read event
    stream->event.SetRead(false);
    stream->event_pool->Update(stream);
    stream->read_buffer = buffer;

    if (!callback) {
      stream->Abort();
    } else {
      callback(stream);
      if (!stream->event.IsRead() && !stream->event.IsWrite()) {
        stream->Abort();
      }
    }
  };
  event.SetRead(true);
  event_pool->Update(shared_from_this());
}

void AsyncTCPStream::ReadUntil(const std::string& str,
                               const AsyncCallback& callback) {
  if (!event_pool) throw std::runtime_error("Event pool is not available");

  read_handler = [callback, str](std::shared_ptr<AsyncTCPStream> stream) {
    char buffer[stream->read_size];
    const int ret =
        ::recv(stream->Descriptor(), buffer, stream->read_size, MSG_DONTWAIT);

    if (ret == -1) {
      throw std::runtime_error(::strerror(errno));
    } else if (ret == 0) {
      // close
      stream->Abort();
      return;
    }

    stream->peek_buffer += buffer;

    auto it = stream->peek_buffer.find(str);
    if (it != std::string::npos) {
      // Finds the target string, refreshes peek buffer and read buffer
      stream->read_buffer = stream->peek_buffer.substr(0, it + str.length());
      stream->peek_buffer = stream->peek_buffer.substr(it + str.length());
      // Disables read event
      stream->event.SetRead(false);
      stream->event_pool->Update(stream);
      if (!callback) {
        stream->Abort();
      } else {
        callback(stream);
        if (!stream->event.IsRead() && !stream->event.IsWrite()) {
          stream->Abort();
        }
      }
    }

    // else keeps looking for the target string
  };

  event.SetRead(true);
  event_pool->Update(shared_from_this());
}

void AsyncTCPStream::ReadLine(const AsyncCallback& cb) { ReadUntil("\n", cb); }

void AsyncTCPStream::Abort() {
  if (!event_pool) throw std::runtime_error("Event pool is not available");

  event_pool->Remove(shared_from_this());
  event_pool = nullptr;
}
}
}
