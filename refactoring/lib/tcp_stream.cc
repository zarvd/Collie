#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include "../inc/tcp_socket.h"
#include "../inc/tcp_stream.h"
#include "../inc/logger.h"

namespace collie {

TcpStream::TcpStream(std::shared_ptr<TcpSocket> peer_fd)
    : peer_fd_(peer_fd), read_size_(3000), status_(OK) {}

TcpStream::~TcpStream() {}

void TcpStream::Write(const std::string& buf) const throw(TcpException) {
  if (status_ == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return;
  }
  char buffer[buf.length() + 1];
  ::strcpy(buffer, buf.c_str());
  const auto len = strlen(buffer);
  if (::send(peer_fd_->GetDescriptor(), buffer, len, 0) == -1) {
    throw TcpException("TCP send");
  }
}
std::string TcpStream::Read() const throw(TcpException) {
  if (status_ == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return "";
  }
  char buffer[read_size_];
  if (::recv(peer_fd_->GetDescriptor(), buffer, read_size_, 0) == -1) {
    throw TcpException("TCP recv");
  }
  return buffer;
}

std::string TcpStream::ReadUntil(const char c) const throw(TcpException) {
  if (status_ == ABORT) {
    LOG(WARN) << "TCP stream is closed";
    return "";
  }
  std::string recv_content;
  while (true) {
    char buffer[read_size_];
    if (::recv(peer_fd_->GetDescriptor(), buffer, read_size_, 0) == -1) {
      throw TcpException("TCP recv");
    }
    recv_content += buffer;
    // find if c in recv buffer;
    int i = 0;
    int buf_size = ::strlen(buffer);
    while (i < buf_size && buffer[i] != c) ++i;
    if (i < buf_size) break;
  }

  return recv_content;
}

std::string TcpStream::ReadLine() const throw(TcpException) {
  return ReadUntil('\n');
}

void TcpStream::Abort() noexcept {
  if (status_ == OK) {
    peer_fd_->Close();

    status_ = ABORT;
  }
}
}
