#ifndef COLLIE_ASYNC_TCP_STREAM_H_
#define COLLIE_ASYNC_TCP_STREAM_H_

#include <memory>
#include "exception.h"
#include "logger.h"
#include "poll/async_io_stream.h"

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
    if (read_handler)
      read_handler(shared_from_this());
    else
      LOG(ERROR) << "read handler is not callable";
  }
  void HandleWrite() noexcept override {
    if (write_handler)
      write_handler(shared_from_this());
    else
      LOG(ERROR) << "read handler is not callable";
  }

  void HandleClose() noexcept override {
    if (close_handler)
      close_handler(shared_from_this());
    else
      LOG(ERROR) << "read handler is not callable";
  }

  void HandleError() noexcept override {
    if (error_handler)
      error_handler(shared_from_this());
    else
      LOG(ERROR) << "read handler is not callable";
  }

  void Write(const std::string&, const AsyncCallback&) throw(TcpException);
  void Read(const AsyncCallback&) throw(TcpException);
  void ReadUntil(const char, const AsyncCallback&) throw(TcpException);
  void ReadLine(const AsyncCallback&) throw(TcpException);
  void Abort() noexcept;

  const Address GetPeerAddress() const noexcept;

  const char* GetReadBuffer() const noexcept { return read_buffer.c_str(); }
  void SetReadSize(unsigned size) noexcept { read_size = size; }
  unsigned GetReadSize() const noexcept { return read_size; }
  Status GetStatus() const noexcept { return status; }

  void SetWriteHander(const AsyncIOHandler& handler) noexcept {
    write_handler = handler;
  }
  void SetReadHander(const AsyncIOHandler& handler) noexcept {
    read_handler = handler;
  }
  void SetCloseHander(const AsyncIOHandler& handler) noexcept {
    close_handler = handler;
  }
  void SetErrorHander(const AsyncIOHandler& handler) noexcept {
    error_handler = handler;
  }

 protected:
  AsyncIOHandler write_handler;
  AsyncIOHandler read_handler;
  AsyncIOHandler error_handler;
  AsyncIOHandler close_handler;

  const std::shared_ptr<TcpSocket> peer_fd;
  unsigned read_size;
  Status status;
  std::string read_buffer;
};
}

#endif /* COLLIE_ASYNC_TCP_STREAM_H_ */
