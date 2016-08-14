#ifndef COLLIE_TCP_STREAM_H_
#define COLLIE_TCP_STREAM_H_

#include <memory>
#include "../base/String.h"
#include "../util/NonCopyable.h"
#include "TCPSocket.h"

namespace collie {
namespace tcp {

class TCPStream : public util::NonCopyable {
 public:
  TCPStream(std::unique_ptr<TCPSocket>) noexcept;
  virtual ~TCPStream();

  virtual void Write(const std::string&) const;
  virtual std::string Read(const SizeType size = 0) const;
  virtual std::string ReadUntil(const std::string&) const;
  virtual std::string ReadLine() const;

  std::shared_ptr<const InetAddress> LocalAddress() const noexcept {
    return socket->LocalAddress();
  }
  std::shared_ptr<const InetAddress> PeerAddress() const noexcept {
    return socket->PeerAddress();
  }

  void SetReadSize(SizeType size) noexcept { read_size = size; }
  SizeType ReadSize() const noexcept { return read_size; }

 private:
  const std::unique_ptr<TCPSocket> socket;
  SizeType read_size;
};
}
}

#endif /* COLLIE_TCP_STREAM_H_ */
