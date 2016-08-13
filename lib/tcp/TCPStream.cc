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
      read_size(3000) {}

TCPStream::~TCPStream() {}

void TCPStream::Write(const String& buf) const {
  int ret = ::send(socket->Descriptor(), buf.RawData(), buf.Length(), 0);
  if (ret == -1) throw std::runtime_error(::strerror(errno));
}

String TCPStream::Read(const SizeType size) const {
  auto read_size = size == 0 ? this->read_size : size;

  char buffer[read_size];
  int ret = ::recv(socket->Descriptor(), buffer, read_size, 0);
  if (ret == -1) throw std::runtime_error(::strerror(errno));
  return buffer;
}

String TCPStream::ReadUntil(const String& str) const {
  static String peek_buffer;
  while (true) {
    char buf[read_size];
    if (::recv(socket->Descriptor(), buf, read_size, 0) == -1) {
      LOG(WARN) << ::strerror(errno);
    }
    peek_buffer += buf;

    auto it = peek_buffer.data.find(str.data);

    if (it != std::string::npos) {
      const String buffer = peek_buffer.Slice(0, it + str.Length());
      peek_buffer = peek_buffer.Slice(it + str.Length());
      return buffer;
    }
  }
}

String TCPStream::ReadLine() const { return ReadUntil("\n"); }
}
}
