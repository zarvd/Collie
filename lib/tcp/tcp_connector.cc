#include "../../include/tcp/tcp_connector.h"
#include "../../include/tcp/tcp_socket.h"
#include "../../include/inet_address.h"
#include "../../include/event/threadpool.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPConnector::TCPConnector(std::shared_ptr<InetAddress> addr)
    : remote_address_(addr) {
  Log(TRACE) << "TCPConnector is constructing";
}

TCPConnector::~TCPConnector() { Log(TRACE) << "TCPConnector is destructing"; }

void TCPConnector::Connect(const size_t thread_num, const size_t connect_num) {
  threadpool_.reset(new event::ThreadPool(thread_num));
  threadpool_->Run();

  for (size_t i = 0; i < connect_num; ++i) {
    threadpool_->Enqueue([this] {
      auto socket = std::make_shared<TCPSocket>(nullptr);
      socket->Connect(remote_address_);
      connect_callback_(socket);
      socket->Close();
    });
  }
}

void TCPConnector::Disconnect() {
  if (threadpool_) threadpool_->terminate();
}
}
}
