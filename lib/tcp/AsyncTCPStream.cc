#include "../../inc/tcp/AsyncTCPStream.h"
#include <sys/socket.h>
#include "../../inc/base/EventPool.h"
#include "../../inc/base/Logger.h"
#include "../../inc/tcp/TCPSocket.h"

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

AsyncTCPStream::~AsyncTCPStream() noexcept {}

int AsyncTCPStream::Descriptor() const noexcept { return socket->Descriptor(); }

std::shared_ptr<const InetAddress> AsyncTCPStream::LocalAddress() const
    noexcept {
  return socket->LocalAddress();
}
std::shared_ptr<const InetAddress> AsyncTCPStream::PeerAddress() const
    noexcept {
  return socket->LocalAddress();
}

void AsyncTCPStream::Write(const String& buf, const AsyncCallback& callback) {
  if (status == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return;
  }
  write_handler = [buf, callback](std::shared_ptr<AsyncTCPStream> stream) {
    if (stream->Status() == ABORT) {
      // FIXME
      LOG(WARN) << "TCP stream is closed";
      return;
    }
    // TODO sendfile
    // Non blocking I/O
    int sent_size =
        ::send(stream->Descriptor(), buf.RawData(), buf.Length(), MSG_DONTWAIT);
    if (sent_size == -1) {
      throw std::runtime_error("tcp cannot send");
    } else if ((SizeType)sent_size < buf.Length()) {
      // Some bytes are not sent, resend the left
      stream->Write(buf.Slice(sent_size), callback);
      return;
    }
    stream->event.SetWrite(false);
    stream->event_pool->Update(stream);
    if (!callback) {
      stream->Abort();
    } else {
      callback(stream);
    }
  };

  event.SetWrite(true);
  if (!event_pool) {
    LOG(ERROR) << "Event pool is not available";
    return;
  }

  event_pool->Update(shared_from_this());
}

void AsyncTCPStream::Read(const AsyncCallback& callback) {
  if (status == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return;
  }

  read_handler = [callback](std::shared_ptr<AsyncTCPStream> stream) {
    char buffer[stream->read_size];
    const int recv_size =
        ::recv(stream->Descriptor(), buffer, stream->read_size, MSG_DONTWAIT);

    if (recv_size == -1) {
      throw std::runtime_error("tcp cannot recv");
    } else if (recv_size == 0) {
      // close
      stream->Abort();
      return;
    }
    LOG(DEBUG) << "recv: " << buffer;
    stream->event.SetRead(false);
    stream->event_pool->Update(stream);
    stream->read_buffer = buffer;

    if (!callback) {
      stream->Abort();
    } else {
      callback(stream);
    }
  };
  event.SetRead(true);
  if (!event_pool) {
    LOG(ERROR) << "Event pool is not available";
    return;
  }
  event_pool->Update(shared_from_this());
}

void AsyncTCPStream::ReadUntil(const char, const AsyncCallback&) {}

void AsyncTCPStream::ReadLine(const AsyncCallback&) {}

void AsyncTCPStream::Abort() noexcept {
  if (!event_pool) {
    LOG(ERROR) << "Event pool is not available";
    return;
  }
  event_pool->Delete(shared_from_this());
  event_pool = nullptr;
}
}
}
