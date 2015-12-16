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

// Non blocking and asynchronous IO stream for TCP.
//
// It manipulates all I/O operations.
//
// It takes the `Channel` of a accepting connection when it constructs and
// closes the `Channel` when it destructs.
//
// A `Channel` is connected to a unique `TCPIOStream`, and the `TCPIOStream`
// instance will be stored in `g_local_thread_tcp_iostream_set`.
//
// You don't need to `Close()` it on your own initiative.
//
// I/O operations:
//   ReadUntil(...)
//   ReadUntilClose(...)
//   Write(...)
class TCPIOStream : public std::enable_shared_from_this<TCPIOStream> {
 public:
  using ReadCallback =
      std::function<void(const std::string &, std::shared_ptr<TCPIOStream>)>;

  using WriteCallback = std::function<void(std::shared_ptr<TCPIOStream>)>;

  TCPIOStream(std::shared_ptr<event::Channel>) noexcept;
  TCPIOStream(const TCPIOStream &) = delete;
  TCPIOStream &operator=(const TCPIOStream &) = delete;
  ~TCPIOStream() noexcept;

  // Asynchronously reads bytes until it finds the given delimiter.
  //
  // If this progress is still running and user call another read event, the new
  // callback will overwrite the old one.
  //
  // If `max_length` is not given, it is set to `default_read_size_`.
  //
  // It closes the connection when it has read more than `max_length` bytes and
  // the delimiter has not been found.
  //
  // It calls `callback` when all `data` is sent. If `callback` is not given,
  // the instance destructs.
  void ReadUntil(const std::string &delimiter, const ReadCallback & = nullptr,
                 const int max_length = -1);
  // Asynchronously read until getting EOF
  void ReadUntilClose(const ReadCallback & = nullptr,
                      const int max_length = -1);
  // Non blocking and asynchronously writes bytes.
  //
  // If `package_length` is not given, it is set as `default_write_size_`.
  //
  // If the length of `data` is over `package_length`, it is sliced.
  //
  // It calls `callback` when all `data` is sent. If `callback` is not given,
  // the instance destructs.
  void Write(const std::string &data, const int package_length = -1,
             const WriteCallback & = nullptr);
  // Closes the stream and `Channel`.
  //
  // This is called when the instance destructs.
  void Close();

 private:
  // `ReadUntil()` sets it to `Channel`'s `ReadCallback`
  void HandleReadUntil(const std::string &delimiter, const ReadCallback &,
                       const unsigned max_length,
                       std::shared_ptr<event::Channel>);
  // `ReadUntilClose()` sets it to `Channel`'s `ReadCallback`
  void HandleReadUntilClose(const ReadCallback &, const unsigned max_length,
                            std::shared_ptr<event::Channel>);
  // `Write()` sets it to `Channel`'s `WriteCallback`
  void HandleWrite(const WriteCallback &, const unsigned package_length,
                   std::shared_ptr<event::Channel>);

  std::shared_ptr<event::Channel> channel_;
  bool is_close_;
  unsigned default_write_size_;
  unsigned default_read_size_;
  std::string read_buffer_;
  std::string write_buffer_;
};

// Stores current thread's `TCPIOStream`.
// `TCPIOStream` is inserted into it when the `TCPIOStream` constructs.
// `TCPIOStream` is erased from it when the `TCPIOStream` destructs.
extern thread_local std::unordered_set<std::shared_ptr<TCPIOStream>>
    g_local_thread_tcp_iostream_set;
}
}

#endif /* COLLIE_TCP_TCP_IOSTREAM_H */
