#include <netinet/in.h>
#include <unistd.h>
#include "../../include/tcp/tcp_socket.h"
#include "../../include/socket.h"
#include "../../include/inet_address.h"
#include "../../include/utils/file.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPSocket::TCPSocket(std::shared_ptr<InetAddress> addr) noexcept
    : Descriptor(-1, false),
      state_(State::Init),
      address_(addr) {
  Log(TRACE) << "TCPSocket is constructing";
  CreateImpl();
}

// construct Accept connection socket
TCPSocket::TCPSocket(const int fd, std::shared_ptr<InetAddress> addr) noexcept
    : Descriptor(fd, true),
      state_(State::Accept),
      address_(addr) {
  Log(TRACE) << "TCPSocket(Accept connection) is constructing";
}

// construct illegal socket
TCPSocket::TCPSocket() noexcept : Descriptor(-1, false),
                                  state_(State::IllegalAccept) {
  Log(TRACE) << "TCPSocket(IllegalAccept) is constructing";
}

TCPSocket::~TCPSocket() noexcept {
  Log(TRACE) << "TCPSocket is destructing";
  CloseImpl();
}

void TCPSocket::Create() noexcept { CreateImpl(); }

void TCPSocket::Close() noexcept { CloseImpl(); }

void TCPSocket::CreateImpl() noexcept {
  if (is_init_) return;
  fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (fd_ < 0) {
    Log(WARN) << "TCPSocket: " << GetError();
  } else {
    state_ = State::Socket;
    is_init_ = true;
    Log(DEBUG) << "TCP Socket " << fd_ << " is created";
  }
}

void TCPSocket::CloseImpl() noexcept {
  Log(TRACE) << "Socket closed";
  if (state_ != State::Init && state_ != State::Close) {
    ::close(fd_);
    state_ = State::Close;
    is_close_ = true;
  }
}

bool TCPSocket::BindAndListen() {
  REQUIRE(address_);
  switch (address_->ip_version()) {
    case IP::V4:
      return ListenV4();
    default:
      return false;
  }
}

bool TCPSocket::ListenV4() {
  if (state_ == State::Socket) {
    struct sockaddr_in servAddr = address_->addr_v4();

    int ret = bind(fd_, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (ret < 0) {
      Log(WARN) << "bind()" << GetError();
    } else {
      state_ = State::Bind;
    }
  }
  if (state_ == State::Bind) {
    Log(TRACE) << "Socket " << fd_ << " is binding";

    Log(TRACE) << "Socket is listening";

    if (::listen(fd_, SOMAXCONN) < 0) {
      Log(WARN) << GetError();
    } else {
      state_ = State::Listen;
    }
  }
  if (state_ == State::Listen) {
    return true;
  } else {
    return false;
  }
}

bool TCPSocket::Connect(std::shared_ptr<InetAddress> serv_address) {
  REQUIRE(serv_address);
  switch (serv_address->ip_version()) {
    case IP::V4:
      return ConnectV4(serv_address);
    case IP::V6:
      Log(WARN) << "IP v6 is not ready to go";
      return false;
    default:
      Log(ERROR) << "IP version is UNKNOWN " << serv_address->ip();
      return false;
  }
}

bool TCPSocket::ConnectV4(std::shared_ptr<InetAddress> serv_address) {
  struct sockaddr_in serv = serv_address->addr_v4();
  if (state_ == State::Socket) {
    int ret = ::connect(fd_, (struct sockaddr *)&serv, sizeof(serv));
    if (ret < 0) {
      Log(WARN) << "connect()" << GetError();
    } else {
      state_ = State::Connect;

      // get local address
      struct sockaddr_in local;
      int addrLen = sizeof(local);
      ::getsockname(fd_, (struct sockaddr *)&local, (socklen_t *)&addrLen);
      address_ = std::make_shared<InetAddress>(local);
      return true;
    }
  } else {
    Log(WARN) << "TCP socket state is not able to connect" << int(state_);
  }
  return false;
}

// Thread safe
std::shared_ptr<TCPSocket> TCPSocket::Accept(bool blocking) noexcept {
  REQUIRE(address_);
  switch (address_->ip_version()) {
    case IP::V4:
      return AcceptV4(blocking);
    default:
      return GetIllegalAcceptSocket();
  }
}

std::shared_ptr<TCPSocket> TCPSocket::AcceptV4(bool blocking) {
  if (state_ == State::Listen) {
    int flags = 0;
    if (blocking) flags = SOCK_NONBLOCK;

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    clientAddrLen = sizeof(clientAddr);

    const int ret =
        ::accept4(fd_, (struct sockaddr *)&clientAddr, &clientAddrLen, flags);
    if (ret < 0) {
      Log(TRACE) << "accept(): " << GetError();
      return GetIllegalAcceptSocket();
    } else {
      Log(DEBUG) << "Socket accept " << ret;
      auto addr = std::make_shared<InetAddress>(clientAddr);
      return GetAcceptSocket(ret, addr);
    }
  }
  return GetIllegalAcceptSocket();
}

ssize_t TCPSocket::Recv(std::string &content, const int flags) {
  REQUIRE(state_ == State::Connect || state_ == State::Accept);
  return socket::Recv(shared_from_this(), content, flags);
}

ssize_t TCPSocket::Send(const std::string &content, const int flags) {
  REQUIRE(state_ == State::Connect || state_ == State::Accept);
  return socket::Send(shared_from_this(), content, flags);
}

bool TCPSocket::SendFile(const utils::File &file) {
  REQUIRE(state_ == State::Connect || state_ == State::Accept);
  return socket::SendFile(shared_from_this(), file);
}

bool TCPSocket::RecvFile(const utils::File &file, const size_t recv_size) {
  REQUIRE(state_ == State::Connect || state_ == State::Accept);
  return socket::RecvFile(shared_from_this(), file, recv_size);
}

std::shared_ptr<TCPSocket> TCPSocket::GetAcceptSocket(
    const int fd, std::shared_ptr<InetAddress> addr) noexcept {
  REQUIRE(fd > 0);
  return std::shared_ptr<TCPSocket>(new TCPSocket(fd, addr));
}

std::shared_ptr<TCPSocket> TCPSocket::GetIllegalAcceptSocket() noexcept {
  static const auto illegalSocket = std::shared_ptr<TCPSocket>(new TCPSocket());
  return illegalSocket;
}
}
}
