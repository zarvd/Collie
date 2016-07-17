#ifndef COLLIE_TCP_STREAM_H_
#define COLLIE_TCP_STREAM_H_

#include <memory>
#include "../base/String.h"
#include "../util/NonCopyable.h"
#include "TCPSocket.h"

namespace collie {
class InetAddress;

namespace tcp {

class TCPStream : public util::NonCopyable {
 public:
  enum StatusType { OK, ABORT };

  TCPStream(std::unique_ptr<TCPSocket>) noexcept;
  virtual ~TCPStream() noexcept;

  virtual void Write(const String&) const;
  virtual String Read() const;
  virtual String ReadUntil(const char) const;
  virtual String ReadLine() const;
  virtual void Abort() noexcept;

  const InetAddress& PeerAddress() const noexcept;

  void SetReadSize(SizeType size) noexcept { read_size = size; }
  SizeType ReadSize() const noexcept { return read_size; }
  StatusType Status() const noexcept { return status; }

 private:
  const std::unique_ptr<TCPSocket> socket;
  SizeType read_size;
  StatusType status;
};
}
}

#endif /* COLLIE_TCP_STREAM_H_ */
