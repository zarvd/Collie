#include "../include/socket.h"
#include "../include/inet_address.h"
#include "../include/utils/file.h"
#include "../include/descriptor.h"
#include "../include/exception.h"
#include "../include/logging.h"
#include <unistd.h>
#include <sys/sendfile.h>

namespace collie {

void socket::SetNonBlocking(std::shared_ptr<Descriptor> descriptor) {
  const auto fd = descriptor->fd();
  auto flags = ::fcntl(fd, F_GETFL, 0);
  REQUIRE_SYS(flags != -1);
  flags |= O_NONBLOCK;
  const int ret = ::fcntl(fd, F_SETFL, flags);
  REQUIRE_SYS(ret != -1);
}

ssize_t socket::Send(std::shared_ptr<Descriptor> descriptor,
                     const std::string &content, const int flags) {
  const auto fd = descriptor->fd();
  char content_chars[content.length() + 1];
  std::strcpy(content_chars, content.c_str());
  Log(TRACE) << "Socket is sending " << content_chars;
  const ssize_t size = ::send(fd, content_chars, sizeof(content_chars), flags);
  REQUIRE_SYS(size != -1);
  if (size == 0) {
    Log(DEBUG) << "Socket send nothing";
  } else {
    Log(TRACE) << "Socket send msg";
  }
  return size;
}

ssize_t socket::Recv(std::shared_ptr<Descriptor> descriptor,
                     std::string &content, const int flags) {
  const auto fd = descriptor->fd();
  if (fd < 2) {
    Log(WARN) << "Illegal socket fd " << fd;
  }
  constexpr size_t kMsgLength = 8192;  // FIXME
  char msg[kMsgLength];
  const ssize_t size = ::recv(fd, msg, kMsgLength, flags);
  REQUIRE_SYS(size != -1);
  if (size == 0) {
    Log(DEBUG) << "Socket received nothing";
    content = "";
  } else {
    Log(TRACE) << "Socket received msg";
    content = msg;
  }

  return size;
}

ssize_t socket::SendTo(std::shared_ptr<Descriptor> descriptor,
                       const std::string &content,
                       std::shared_ptr<InetAddress> remote_address,
                       const int flags) {
  const auto fd = descriptor->fd();
  char content_chars[content.length() + 1];
  std::strcpy(content_chars, content.c_str());
  struct sockaddr_in addr = remote_address->addr_v4();
  const ssize_t size =
      ::sendto(fd, content_chars, sizeof(content_chars), flags,
               (struct sockaddr *)&addr, sizeof(remote_address->addr_v4()));
  REQUIRE_SYS(size != -1);
  if (size == 0) {
    Log(DEBUG) << "Socket send nothing";
  } else {
    Log(TRACE) << "Socket send msg";
  }
  return size;
}

ssize_t socket::RecvFrom(std::shared_ptr<Descriptor> descriptor,
                         std::string &content,
                         const std::shared_ptr<InetAddress> &remote_address,
                         const int flags) {
  const auto fd = descriptor->fd();
  char buffer[8192];
  struct sockaddr_in addr;
  socklen_t addr_length = sizeof(addr);
  const ssize_t size = ::recvfrom(fd, buffer, sizeof(buffer), flags,
                                  (struct sockaddr *)&addr, &addr_length);
  REQUIRE_SYS(size != -1);
  if (size == 0) {
    Log(DEBUG) << "Socket received nothing";
    content = "";
  } else {
    Log(TRACE) << "Socket received msg";
    content = buffer;
    (*remote_address) = addr;
  }
  return size;
}

bool socket::SendFile(std::shared_ptr<Descriptor> descriptor,
                      const utils::File &file) {
  const auto fd = descriptor->fd();
  REQUIRE(file.is_existed() && file.IsRead());
  if (file.is_existed() && file.IsFile()) {
    off64_t offset = 0;
    size_t sent_size = 0;
    while (sent_size < file.GetSize()) {
      const ssize_t ret = ::sendfile64(fd, file.fd(), &offset, file.GetSize());
      Log(DEBUG) << "sendfile64() return " << ret;
      if (ret == -1) {
        Log(ERROR) << GetError();
        return false;
      }
      sent_size += static_cast<size_t>(ret);
    }
    return true;
  } else {
    return false;
  }
}

bool socket::RecvFile(std::shared_ptr<Descriptor> descriptor,
                      const utils::File &file, const size_t file_size) {
  const auto fd = descriptor->fd();
  REQUIRE(file.is_existed() && file.IsWrite());
  if (file.is_existed() && file.IsFile()) {
    char buffer[file_size];
    int ret = ::recv(fd, buffer, file_size, MSG_WAITALL);
    REQUIRE_SYS(ret != -1);
    ret = ::write(file.fd(), buffer, ret);
    REQUIRE_SYS(ret != -1);
    return true;
  } else {
    return false;
  }
}
}
