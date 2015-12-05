#ifndef COLLIE_TCP_TCP_IOSTREAM_H
#define COLLIE_TCP_TCP_IOSTREAM_H

#include <unordered_set>
#include <memory>
#include <functional>
#include <string>

namespace collie {
namespace event {
class Channel;
}

namespace tcp {

// TCP non blocking IO stream
class TCPIOStream : public std::enable_shared_from_this<TCPIOStream> {
 public:
  using ReadCallback =
      std::function<void(const std::string &, std::shared_ptr<TCPIOStream>)>;

  using WriteCallback = std::function<void(std::shared_ptr<TCPIOStream>)>;

  TCPIOStream(std::shared_ptr<event::Channel>) noexcept;
  TCPIOStream(const TCPIOStream &) = delete;
  TCPIOStream &operator=(const TCPIOStream &) = delete;
  ~TCPIOStream() noexcept;

  void ReadUntil(const std::string &delimiter, const ReadCallback & = nullptr,
                 const int max_length = -1);
  void ReadUntilClose(const ReadCallback & = nullptr,
                      const int max_length = -1);
  void Write(const std::string &data, const int package_length = -1,
             const WriteCallback & = nullptr);
  void Close();

 private:
  void HandleReadUntil(const std::string &delimiter, const ReadCallback &,
                       const unsigned max_length,
                       std::shared_ptr<event::Channel>);
  void HandleReadUntilClose(const ReadCallback &, const unsigned max_length,
                            std::shared_ptr<event::Channel>);
  void HandleWrite(const WriteCallback &, const unsigned package_length,
                   std::shared_ptr<event::Channel>);

  std::shared_ptr<event::Channel> channel_;
  bool is_close_;
  ReadCallback read_callback_;
  WriteCallback write_callback_;
  unsigned default_write_size_;
  unsigned default_read_size_;
  std::string read_buffer_;
  std::string write_buffer_;
};

extern thread_local std::unordered_set<std::shared_ptr<TCPIOStream>>
    g_local_thread_tcp_iostream_set;
}
}

#endif /* COLLIE_TCP_TCP_IOSTREAM_H */
