#ifndef COLLIE_TCP_STREAM_H_
#define COLLIE_TCP_STREAM_H_

#include <memory>
#include <string.h>
#include "util/noncopyable.h"
#include "exception.h"

namespace collie {

class InetAddress;
class TcpSocket;

class TcpStream : public NonCopyable {
 public:
  enum Status { OK, ABORT };
  using Address = std::shared_ptr<InetAddress>;

  TcpStream(std::shared_ptr<TcpSocket>);
  virtual ~TcpStream();

  virtual void Write(const std::string&) const throw(TcpException);
  virtual std::string Read() const throw(TcpException);
  // virtual std::string ReadUntil(const char) const throw(TcpException);
  // virtual std::string ReadLine() const throw(TcpException);
  virtual void Abort() noexcept;

  const Address peer_address() const noexcept;

  void set_read_size(unsigned size) noexcept { read_size_ = size; }
  unsigned read_size() const noexcept { return read_size_; }
  Status status() const noexcept { return status_; }

 protected:
  const std::shared_ptr<TcpSocket> peer_fd_;
  unsigned read_size_;
  Status status_;
};
}

#endif /* COLLIE_TCP_STREAM_H_ */
