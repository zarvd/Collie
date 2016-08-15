#include "../../inc/http/HTTPServer.h"
#include "../../inc/base/Logger.h"
#include "../../inc/http/HTTPHeader.h"
#include "../../inc/http/HTTPRequest.h"
#include "../../inc/tcp/TCPServer.h"

namespace collie {
namespace http {

HTTPServer::HTTPServer(const unsigned port, const std::string& host)
    : req_handler(nullptr),
      tcp_server(std::make_unique<tcp::TCPServer>()),
      port(port),
      host(host) {}

HTTPServer::~HTTPServer() {}

void HTTPServer::Start() {
  tcp_server->Listen(port, host);
  tcp_server->SetRequestHandler([http = this](tcp::TCPStream & stream) {
    auto http_message = stream.ReadUntil("\r\n\r\n");
    unsigned content_length = 0;
    try {
      HTTPRequest request(http_message);
      content_length = std::stoi(request.Header()->Get("Content-Length"));
    } catch (std::runtime_error& e) {
      LOG(ERROR) << "http request parse error";
      return;
    }

    auto http_body = stream.Read(content_length);

  });
  tcp_server->Start();
}
}
}
