#pragma once

#include <collie/Bytes.h>
#include <collie/NonCopyable.h>
#include <memory>
#include "TCPSocket.h"

namespace collie {

class TCPStream final : public NonCopyable {
 public:
  explicit TCPStream(std::unique_ptr<TCPSocket>) noexcept;
  virtual ~TCPStream();

  void Write(const std::string&) const;
  void Write(const Bytes&) const;
  Bytes ReadBytes(const unsigned size) const;
  Bytes ReadBytesUntil(const Bytes&) const;
  std::string Read(const unsigned size = 0) const;
  std::string ReadUntil(const std::string&) const;
  std::string ReadLine() const;

  std::shared_ptr<const InetAddress> LocalAddress() const noexcept {
    return socket->LocalAddress();
  }
  std::shared_ptr<const InetAddress> PeerAddress() const noexcept {
    return socket->PeerAddress();
  }

  void SetReadSize(unsigned size) noexcept { read_size = size; }
  unsigned ReadSize() const noexcept { return read_size; }

 private:
  const std::unique_ptr<TCPSocket> socket;
  unsigned read_size;
};
}
