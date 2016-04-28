#ifndef COLLIE_TCP_STREAM_H_
#define COLLIE_TCP_STREAM_H_

#include <memory>
#include <string.h>
#include "util/noncopyable.h"
#include "exception.h"

namespace collie {

class InetAddress;

class TcpStream : public NonCopyable {
 public:
  enum Status { OK, ABORT };
  using Address = std::shared_ptr<InetAddress>;

  TcpStream(Address local_address, Address peer_address, int peer_fd);
  virtual ~TcpStream();

  virtual void Write(const std::string&) const throw(TcpException);
  virtual std::string Read() const throw(TcpException);
  virtual void Abort() noexcept;

  const Address local_address() const noexcept { return local_address_; }
  const Address peer_address() const noexcept { return peer_address_; }

  void set_read_size(unsigned size) noexcept { read_size_ = size; }
  unsigned read_size() const noexcept { return read_size_; }
  Status status() const noexcept { return status_; }

 private:
  const int peer_fd_;
  const Address local_address_;
  const Address peer_address_;
  unsigned read_size_;
  Status status_;
};
}

#endif /* COLLIE_TCP_STREAM_H_ */
