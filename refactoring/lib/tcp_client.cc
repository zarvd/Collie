#include "../inc/inet_address.h"
#include "../inc/tcp_client.h"
#include "../inc/tcp_stream.h"
#include "../inc/tcp_socket.h"
#include <sys/socket.h>

namespace collie {

void TcpClient::Connect(const Host &host, const Port &port,
                        const Request &req) throw(TcpException) {
  auto serv_addr = InetAddress::GetInetAddress(host, port);
  Connect(serv_addr, req);
}

void TcpClient::Connect(Address serv_addr,
                        const Request &req) throw(TcpException) {
  auto serv_socket = std::make_shared<TcpSocket>();
  serv_socket->Connect(serv_addr);

  TcpStream tcp_stream(serv_socket);
  req(tcp_stream);
}
}
