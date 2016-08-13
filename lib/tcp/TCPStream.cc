#include "../../inc/tcp/TCPStream.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include "../../inc/base/Logger.h"
#include "../../inc/tcp/TCPSocket.h"

namespace collie {
namespace tcp {

TCPStream::TCPStream(std::unique_ptr<TCPSocket> socket) noexcept
    : socket(std::move(socket)),
      read_size(3000),
      status(OK) {}

TCPStream::~TCPStream() {}

void TCPStream::Write(const String& buf) const {
  if (status == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return;
  }
  char buffer[buf.Length() + 1];
  ::strcpy(buffer, buf.RawData());
  const auto len = strlen(buffer);
  if (::send(socket->Descriptor(), buffer, len, 0) == -1) {
    LOG(WARN) << "TCP cannot send";
  }
}

String TCPStream::Read(const SizeType size) const {
  if (status == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return "";
  }
  auto read_size = size == 0 ? this->read_size : size;

  char buffer[read_size];
  if (::recv(socket->Descriptor(), buffer, read_size, 0) == -1) {
    LOG(WARN) << "TCP recv";
  }
  return buffer;
}

String TCPStream::ReadUntil(const String& str) const {
  if (status == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return "";
  }
  String recv_content;
  while (true) {
    char buffer[read_size];
    if (::recv(socket->Descriptor(), buffer, read_size, 0) == -1) {
      LOG(WARN) << "TCP recv";
    }
    recv_content += buffer;
  }

  return recv_content;
}

String TCPStream::ReadLine() const { return ReadUntil("\n"); }

void TCPStream::Abort() noexcept {
  if (status == OK) {
    status = ABORT;
  }
}
}
}
