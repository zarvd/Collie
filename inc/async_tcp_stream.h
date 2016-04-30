#ifndef COLLIE_ASYNC_TCP_STREAM_H_
#define COLLIE_ASYNC_TCP_STREAM_H_

#include <memory>
#include "poll/async_io_stream.h"
#include "exception.h"
#include "logger.h"

namespace collie {

class TcpSocket;
class InetAddress;

// Should be shared pointer
class AsyncTcpStream : public AsyncIOStream,
                       public std::enable_shared_from_this<AsyncTcpStream> {
 public:
  enum Status { OK, ABORT, READ, READ_UNTIL, READ_LINE, WRITE };
  using AsyncIOHandler = std::function<void(std::shared_ptr<AsyncTcpStream>)>;
  using AsyncCallback = std::function<void(std::shared_ptr<AsyncTcpStream>)>;
  using Address = std::shared_ptr<InetAddress>;

  AsyncTcpStream(std::shared_ptr<TcpSocket>) noexcept;
  ~AsyncTcpStream() noexcept override;

  int GetDescriptor() const noexcept override;
  void HandleRead() noexcept override {
    if (read_handler_)
      read_handler_(shared_from_this());
    else
      LOG(ERROR) << "read handler is not callable";
  }
  void HandleWrite() noexcept override {
    if (write_handler_)
      write_handler_(shared_from_this());
    else
      LOG(ERROR) << "read handler is not callable";
  }

  void HandleClose() noexcept override {
    if (close_handler_)
      close_handler_(shared_from_this());
    else
      LOG(ERROR) << "read handler is not callable";
  }

  void HandleError() noexcept override {
    if (error_handler_)
      error_handler_(shared_from_this());
    else
      LOG(ERROR) << "read handler is not callable";
  }

  void Write(const std::string&, const AsyncCallback&) throw(TcpException);
  void Read(const AsyncCallback&) throw(TcpException);
  void ReadUntil(const char, const AsyncCallback&) throw(TcpException);
  void ReadLine(const AsyncCallback&) throw(TcpException);
  void Abort() noexcept;

  const Address peer_address() const noexcept;

  const char* read_buffer() const noexcept { return read_buffer_.c_str(); }
  void set_read_size(unsigned size) noexcept { read_size_ = size; }
  unsigned read_size() const noexcept { return read_size_; }
  Status status() const noexcept { return status_; }

  void set_write_hander(const AsyncIOHandler& handler) noexcept {
    write_handler_ = handler;
  }
  void set_read_hander(const AsyncIOHandler& handler) noexcept {
    read_handler_ = handler;
  }
  void set_close_hander(const AsyncIOHandler& handler) noexcept {
    close_handler_ = handler;
  }
  void set_error_hander(const AsyncIOHandler& handler) noexcept {
    error_handler_ = handler;
  }

 protected:
  AsyncIOHandler write_handler_;
  AsyncIOHandler read_handler_;
  AsyncIOHandler error_handler_;
  AsyncIOHandler close_handler_;

  const std::shared_ptr<TcpSocket> peer_fd_;
  unsigned read_size_;
  Status status_;
  std::string read_buffer_;
};
}

#endif /* COLLIE_ASYNC_TCP_STREAM_H_ */
