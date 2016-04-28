#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "../inc/tcp_stream.h"
#include "../inc/logger.h"

namespace collie {

TcpStream::TcpStream(Address local_address, Address peer_address, int peer_fd)
    : peer_fd_(peer_fd),
      local_address_(local_address),
      peer_address_(peer_address),
      read_size_(3000),
      status_(OK) {}

TcpStream::~TcpStream() {}

void TcpStream::Write(const std::string& buf) const throw(TcpException) {
  if (status_ == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return;
  }
  char buffer[buf.length() + 1];
  ::strcpy(buffer, buf.c_str());
  const auto len = strlen(buffer);
  if (::send(peer_fd_, buffer, len, 0) == -1) {
    throw TcpException("TCP send");
  }
}
std::string TcpStream::Read() const throw(TcpException) {
  if (status_ == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return "";
  }
  char buffer[read_size_];
  if (::recv(peer_fd_, buffer, read_size_, 0) == -1) {
    throw TcpException("TCP recv");
  }
  return buffer;
}

void TcpStream::Abort() noexcept {
  if (status_ == OK) {
    if (::close(peer_fd_) == -1) {
      LOG(WARN) << "TCP close: " << ::strerror(errno);
    }
    status_ = ABORT;
  }
}
}
