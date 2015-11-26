#include "../../include/tcp/tcp_client.h"
#include "../../include/tcp/tcp_connector.h"
#include "../../include/inet_address.h"
#include "../../include/exception.h"
#include "../../include/logging.h"

namespace collie {
namespace tcp {

TCPClient::TCPClient() { Log(TRACE) << "TCPClient is constructing"; }

TCPClient::~TCPClient() {
  Log(TRACE) << "TCPClient is destructing";
  if (connector_) connector_->Disconnect();
}

void TCPClient::Connect(const std::string& host, const unsigned port,
                        const size_t thread_num, const size_t connect_num) {
  if (!connect_callback_) {
    THROW_("TCPClient connectCallback is not callable");
  }

  remote_address_ = InetAddress::GetInetAddress(host, port);

  connector_.reset(new TCPConnector(remote_address_));
  connector_->set_connect_callback(connect_callback_);
  connector_->Connect(thread_num, connect_num);
}
}
}
