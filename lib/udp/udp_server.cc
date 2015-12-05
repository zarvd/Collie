#include "../../include/udp/udp_server.h"
#include "../../include/udp/udp_socket.h"
#include "../../include/inet_address.h"
#include "../../include/logging.h"

namespace collie {
namespace udp {

UDPServer::UDPServer() {}
UDPServer::~UDPServer() {}

void UDPServer::Bind(const std::string& host, const unsigned port) {
  local_address_ = InetAddress::GetInetAddress(host, port);
  socket_.reset(new UDPSocket(local_address_));
  socket_->Listen();
}

void UDPServer::Start() {
  LOG(INFO) << "UDP Server is starting";
  CHECK(connect_callback_);
  while (true) {
    std::string content;
    auto remote_address = std::make_shared<InetAddress>();
    // socket_->recvFrom(content, remote_address);
    connect_callback_(content, remote_address);
  }
}

void UDPServer::Send(const std::string& content,
                     std::shared_ptr<InetAddress> remote_address) {
  CHECK(socket_);
  // socket_->sendTo(content, remote_address);
}
}
}
