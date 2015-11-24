#ifndef COLLIE_TCP_TCPCONNECTION_H_
#define COLLIE_TCP_TCPCONNECTION_H_

#include <string>
#include <memory>
#include <unordered_set>

namespace collie {

class InetAddress;

namespace utils {
class File;
}

namespace event {

class EventLoop;
class Channel;
}

namespace tcp {

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
 public:
  using MessageCallback = std::function<void(std::shared_ptr<TCPConnection>)>;
  using EventCallback = std::function<void(std::shared_ptr<TCPConnection>)>;

  explicit TCPConnection(std::shared_ptr<event::Channel> channel,
                         std::shared_ptr<InetAddress> local_address = nullptr);
  TCPConnection(const TCPConnection &) = delete;
  TCPConnection &operator=(const TCPConnection &) = delete;
  ~TCPConnection();

  std::shared_ptr<event::Channel> channel() const { return channel_; }
  std::shared_ptr<InetAddress> local_address() const { return local_address_; }
  std::shared_ptr<InetAddress> remote_address() const {
    return remote_address_;
  }

  void set_message_callback(const MessageCallback &cb) {
    message_callback_ = cb;
  }
  void set_message_callback(const MessageCallback &&cb) {
    message_callback_ = std::move(cb);
  }

  void set_terminate_callback(const EventCallback &cb) {
    terminate_callback_ = cb;
  }
  void set_terminate_callback(const EventCallback &&cb) {
    terminate_callback_ = std::move(cb);
  }

  void Disconnect();
  std::string RecvAll();
  void Send(const std::string &);
  void SendFile(const std::string &file_path);
  void SendFile(const utils::File &file);
  void RecvFile(const std::string &file_path, const size_t recv_size);
  void RecvFile(const utils::File &file, const size_t recv_size);

 private:
  void Terminate();
  void HandleRead();
  void HandleWrite();
  void HandleClose();
  void HandleError();

  bool connected_;
  bool terminate_;
  const std::shared_ptr<event::Channel> channel_;
  std::shared_ptr<InetAddress> local_address_;
  std::shared_ptr<InetAddress> remote_address_;
  std::string inputBuffer_;
  std::string outputBuffer_;
  MessageCallback message_callback_;
  EventCallback terminate_callback_;
};

extern thread_local std::unordered_set<std::shared_ptr<TCPConnection>>
    g_local_thread_connections;
}
}

#endif /* COLLIE_TCP_TCPCONNECTION_H_ */
