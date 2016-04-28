#ifndef COLLIE_TCP_STREAM_H_
#define COLLIE_TCP_STREAM_H_

#include <memory>
#include <string.h>
#include "util/noncopyable.h"

namespace collie {

class TcpException : public std::runtime_error {
 public:
  explicit TcpException(const std::string& message) noexcept
      : std::runtime_error(message + ": " + ::strerror(errno)) {}
  ~TcpException() noexcept override {}
};

class InetAddress;

class TcpStream : public NonCopyable {
 public:
  using Address = std::shared_ptr<InetAddress>;

  TcpStream(Address local_address, Address peer_address, int peer_fd);
  virtual ~TcpStream();

  virtual void Write(const std::string&) const throw(TcpException);
  virtual std::string Read() const throw(TcpException);

  const Address local_address() const noexcept { return local_address_; }
  const Address peer_address() const noexcept { return peer_address_; }

  void set_read_size(unsigned size) noexcept { read_size_ = size; }
  unsigned read_size() const noexcept { return read_size_; }

 private:
  const int peer_fd_;
  const Address local_address_;
  const Address peer_address_;
  unsigned read_size_;
};
}

#endif /* COLLIE_TCP_STREAM_H_ */
