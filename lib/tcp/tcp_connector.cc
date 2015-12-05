#include "../../include/tcp/tcp_connector.h"
#include "../../include/tcp/tcp_socket.h"
#include "../../include/inet_address.h"
#include "../../include/event/threadpool.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPConnector::TCPConnector(std::shared_ptr<InetAddress> addr) noexcept
    : remote_address_(addr) {
}

TCPConnector::~TCPConnector() noexcept {
}

void TCPConnector::Connect(const size_t thread_num, const size_t connect_num) {
  threadpool_.reset(new event::ThreadPool(thread_num));
  threadpool_->Run();

  for (size_t i = 0; i < connect_num; ++i) {
    threadpool_->Enqueue([
      connect_callback = connect_callback_,
      remote_address = remote_address_
    ] {
      auto socket = std::make_shared<TCPSocket>(nullptr);
      if (socket->Connect(remote_address)) {
        connect_callback(socket);
      } else {
        LOG(WARNING) << "TCP client socket cannot connect";
      }
      socket->Close();
    });
  }
}

void TCPConnector::Disconnect() {
  if (threadpool_) threadpool_->terminate();
}
}
}
