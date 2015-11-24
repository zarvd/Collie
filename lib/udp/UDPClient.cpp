#include "../../include/inet_address.h"
#include "../../include/udp/udp_socket.h"
#include "../../include/udp/udp_client.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace udp {

UDPClient::UDPClient() {}
UDPClient::~UDPClient() {}

void UDPClient::Connect(const std::string& host, const unsigned port,
                        const std::string& greeting) {
  REQUIRE(connect_callback_);
  socket_.reset(new UDPSocket(nullptr));
  remote_address_ = InetAddress::GetInetAddress(host, port);
  socket_->Connect(remote_address_->ip_version());
  // socket_->sendTo(greeting, remote_address_);
  std::string content;
  // socket_->recvFrom(content, remote_address_);
  while (connect_callback_(content, remote_address_)) {
    // loop until return false
  }
}

void UDPClient::Send(const std::string& content,
                     std::shared_ptr<InetAddress> remote_address_) {
  REQUIRE(socket_);
  // socket_->sendTo(content, remote_address_);
}
}
}
