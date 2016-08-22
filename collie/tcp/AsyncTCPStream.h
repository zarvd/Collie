#pragma once

#include <collie/AsyncIOStream.h>
#include <collie/Logger.h>
#include <collie/tcp/TCPSocket.h>
#include <memory>

namespace collie {

// Should be shared pointer
class AsyncTCPStream final
    : public AsyncIOStream,
      public std::enable_shared_from_this<AsyncTCPStream> {
 public:
  using AsyncIOHandler = std::function<void(std::shared_ptr<AsyncTCPStream>)>;
  using AsyncCallback = std::function<void(std::shared_ptr<AsyncTCPStream>)>;

  explicit AsyncTCPStream(std::unique_ptr<TCPSocket>) noexcept;
  ~AsyncTCPStream() override;

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

  int Descriptor() const noexcept override;
  void Write(const std::string&, const AsyncCallback&);
  void Read(const AsyncCallback&);
  void ReadUntil(const std::string&, const AsyncCallback&);
  void ReadLine(const AsyncCallback&);
  void Abort();

  std::shared_ptr<const InetAddress> LocalAddress() const noexcept;
  std::shared_ptr<const InetAddress> PeerAddress() const noexcept;

  std::string ReadBuffer() const noexcept { return read_buffer; }
  void ReadSize(unsigned size) noexcept { read_size = size; }
  unsigned ReadSize() const noexcept { return read_size; }

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

 private:
  AsyncIOHandler write_handler;
  AsyncIOHandler read_handler;
  AsyncIOHandler error_handler;
  AsyncIOHandler close_handler;

  const std::unique_ptr<TCPSocket> socket;
  unsigned read_size;
  std::string read_buffer;
  std::string peek_buffer;
};
}
