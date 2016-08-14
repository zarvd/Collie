#include "../../inc/http/HTTPServer.h"
#include "../../inc/http/HTTPHeader.h"
#include "../../inc/tcp/TCPServer.h"

namespace collie {
namespace http {

HTTPServer::HTTPServer(const unsigned port, const String& host)
    : req_handler(nullptr),
      tcp_server(std::make_unique<tcp::TCPServer>()),
      port(port),
      host(host) {}

HTTPServer::~HTTPServer() {}

void HTTPServer::Start() {
  tcp_server->Listen(port, host);
  tcp_server->SetRequestHandler([http = this](tcp::TCPStream & stream) {
    auto http_message = stream.ReadUntil("\r\n\r\n");
  });
  tcp_server->Start();
}
}
}
