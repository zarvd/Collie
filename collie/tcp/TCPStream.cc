#include <collie/Logger.h>
#include <collie/tcp/TCPSocket.h>
#include <collie/tcp/TCPStream.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

namespace collie {

TCPStream::TCPStream(std::unique_ptr<TCPSocket> socket) noexcept
    : socket(std::move(socket)),
      read_size(3000) {}

TCPStream::~TCPStream() {}

void TCPStream::Write(const std::string& buf) const {
  int ret = ::send(socket->Descriptor(), buf.c_str(), buf.length(), 0);
  if (ret == -1) throw std::runtime_error(::strerror(errno));
}

void TCPStream::Write(const Bytes& buf) const {
  int ret = ::send(socket->Descriptor(), buf.Content(), buf.Size(), 0);
  if (ret == -1) throw std::runtime_error(::strerror(errno));
}

Bytes TCPStream::ReadBytes(const unsigned size) const {
  auto read_size = size == 0 ? this->read_size : size;

  unsigned char buffer[read_size];
  int ret = ::recv(socket->Descriptor(), buffer, read_size, 0);
  if (ret == -1) throw std::runtime_error(::strerror(errno));

  return Bytes(buffer, ret);
}

std::string TCPStream::Read(const unsigned size) const {
  auto read_size = size == 0 ? this->read_size : size;

  char buffer[read_size];
  int ret = ::recv(socket->Descriptor(), buffer, read_size, 0);
  if (ret == -1) throw std::runtime_error(::strerror(errno));
  return buffer;
}

std::string TCPStream::ReadUntil(const std::string& str) const {
  static std::string peek_buffer;
  while (true) {
    char buf[read_size];
    if (::recv(socket->Descriptor(), buf, read_size, 0) == -1) {
      LOG(WARN) << ::strerror(errno);
    }
    peek_buffer += buf;

    auto it = peek_buffer.find(str);

    if (it != std::string::npos) {
      const std::string buffer = peek_buffer.substr(0, it + str.length());
      peek_buffer = peek_buffer.substr(it + str.length());
      return buffer;
    }
  }
}

std::string TCPStream::ReadLine() const { return ReadUntil("\n"); }
}
