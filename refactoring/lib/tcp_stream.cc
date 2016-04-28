#include <sys/socket.h>
#include "../inc/tcp_stream.h"
#include "string.h"

namespace collie {

TcpStream::TcpStream(Address local_address, Address peer_address, int peer_fd)
    : peer_fd_(peer_fd),
      local_address_(local_address),
      peer_address_(peer_address),
      read_size_(3000) {}

TcpStream::~TcpStream() {}

void TcpStream::Write(const std::string& buf) const throw(TcpException) {
  char buffer[buf.length() + 1];
  ::strcpy(buffer, buf.c_str());
  const auto len = strlen(buffer);
  if (::send(peer_fd_, buffer, len, 0) == -1) {
    throw TcpException("TCP send");
  }
}
std::string TcpStream::Read() const throw(TcpException) {
  char buffer[read_size_];
  if (::recv(peer_fd_, buffer, read_size_, 0) == -1) {
    throw TcpException("TCP recv");
  }
  return buffer;
}
}
