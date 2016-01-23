#include <netinet/tcp.h>
#include <unistd.h>
#include "../../include/tcp/tcp_socket.h"
#include "../../include/inet_address.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPSocket::TCPSocket() : Descriptor() { CreateImpl(); }

TCPSocket::~TCPSocket() { CloseImpl(); }

void TCPSocket::Create() { CreateImpl(); }

void TCPSocket::Close() { CloseImpl(); }

void TCPSocket::CreateImpl() {
  if (is_init_) return;
  fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (fd_ == -1) {
    LOG(WARNING) << "TCPSocket: " << GetSystemError();
    state_ = State::Error;
  } else {
    is_init_ = true;
    state_ = State::Unconnected;
  }
}

void TCPSocket::CloseImpl() {
  if (state_ != State::Error && state_ != State::Closed) {
    ::close(fd_);
    state_ = State::Closed;
    is_close_ = true;
  }
}

bool TCPSocket::SetNoDelay(const bool enable) const {
  if (state_ == State::Unconnected || state_ == State::Closed) {
    return false;
  }
  int value = enable ? 1 : 0;
  const int ret =
      ::setsockopt(fd_, SOL_TCP, TCP_NODELAY, &value, sizeof(value));
  if (ret == -1) {
    LOG(WARNING) << "setsockopt() setting TCP_NODELAY: " << GetSystemError();
    return false;
  } else {
    return true;
  }
}

bool TCPSocket::Bind(const std::shared_ptr<InetAddress> local_address) {
  if (!local_address) return false;
  local_address_ = local_address;
  switch (local_address_->ip_version()) {
    case IP::V4: {
      if (state_ == State::Unconnected) {
        struct sockaddr_in servAddr = local_address_->addr_v4();

        int ret = ::bind(fd_, (struct sockaddr *)&servAddr, sizeof(servAddr));
        if (ret == -1) {
          LOG(WARNING) << "bind()" << GetSystemError();
        } else {
          state_ = State::Bound;
        }
      }
      if (state_ == State::Bound) {
        if (::listen(fd_, SOMAXCONN) == -1) {
          LOG(WARNING) << GetSystemError();
        } else {
          state_ = State::Listening;
        }
      }
      if (state_ == State::Listening) {
        return true;
      } else {
        return false;
      }
    }
    default:
      return false;
  }
}

bool TCPSocket::ConnectTo(std::shared_ptr<InetAddress> peer_address) {
  CHECK(peer_address);
  switch (peer_address->ip_version()) {
    case IP::V4:
      return ConnectToV4(peer_address);
    case IP::V6:
      LOG(WARNING) << "IP v6 is not ready to go";
      return false;
    default:
      LOG(ERROR) << "IP version is UNKNOWN " << peer_address->ip();
      return false;
  }
}

bool TCPSocket::ConnectToV4(std::shared_ptr<InetAddress> peer_address) {
  struct sockaddr_in serv = peer_address->addr_v4();
  if (state_ == State::Unconnected || state_ == State::Bound) {
    int ret = ::connect(fd_, (struct sockaddr *)&serv, sizeof(serv));
    if (ret == -1) {
      LOG(WARNING) << "connect()" << GetSystemError();
    } else {
      if (state_ == State::Unconnected) {
        // get local address
        struct sockaddr_in local;
        int addrLen = sizeof(local);
        ::getsockname(fd_, (struct sockaddr *)&local, (socklen_t *)&addrLen);
        local_address_ = std::make_shared<InetAddress>(local);
      }
      peer_address_ = peer_address;
      state_ = State::Connected;
      return true;
    }
  } else {
    LOG(WARNING) << "TCP socket state is not able to connect";
  }
  return false;
}

// Thread safe
std::shared_ptr<TCPSocket> TCPSocket::Accept(bool blocking) {
  CHECK(local_address_);
  switch (local_address_->ip_version()) {
    case IP::V4:
      return AcceptV4(blocking);
    default:
      return GetErrorSocket();
  }
}

std::shared_ptr<TCPSocket> TCPSocket::AcceptV4(bool blocking) {
  if (state_ == State::Listening) {
    int flags = 0;
    if (blocking) flags = SOCK_NONBLOCK;

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    clientAddrLen = sizeof(clientAddr);

    const int ret =
        ::accept4(fd_, (struct sockaddr *)&clientAddr, &clientAddrLen, flags);
    if (ret == -1) {
      LOG(TRACE) << "accept(): " << GetSystemError();
      return GetErrorSocket();
    } else {
      LOG(DEBUG) << "Socket accept " << ret;
      auto addr = std::make_shared<InetAddress>(clientAddr);
      return GetConnectedSocket(ret, addr);
    }
  }
  return GetErrorSocket();
}

std::shared_ptr<TCPSocket> TCPSocket::GetConnectedSocket(
    const int fd, std::shared_ptr<InetAddress> peer_address) {
  auto connected_socket = std::make_shared<TCPSocket>();
  connected_socket->state_ = State::Connected;
  connected_socket->fd_ = fd;
  connected_socket->peer_address_ = peer_address;
  return connected_socket;
}

std::shared_ptr<TCPSocket> TCPSocket::GetErrorSocket() {
  static const auto error_socket = std::make_shared<TCPSocket>();
  error_socket->state_ = State::Error;
  return error_socket;
}

bool TCPSocket::Write(const std::string &data) {
  char buffer[data.length()];
  std::strncpy(buffer, data.c_str(), sizeof(buffer));
  const auto ret = ::send(fd_, buffer, sizeof(buffer), 0);
  if (ret == -1) {
    return false;
  } else {
    return true;
  }
}

std::string TCPSocket::Read(const ssize_t size) {
  char content[size];
  const auto ret = ::recv(fd_, content, size, 0);
  if (ret == -1) {
    return std::string();
  } else {
    return content;
  }
}
}
}
