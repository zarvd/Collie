#include "../include/udp/udp_socket.h"
#include "../include/udp/udp_server.h"
#include "../include/inet_address.h"
#include "../include/exception.h"
#include "../include/logging.h"

using collie::udp::UDPServer;
using namespace collie;

int main(int argc, char* argv[]) {
  auto& logging = logger::LogHandler::GetHandler();
  logging.set_log_level(TRACE);
  logging.Init();

  unsigned port = 8080;
  if (argc == 2) port = std::stoul(argv[1]);

  UDPServer server;
  server.Bind("127.0.0.1", port);
  server.set_connect_callback(
      [&server](const std::string& content, std::shared_ptr<InetAddress> addr) {
        Log(INFO) << "Server received: " << content;
        const auto res = "From UDP Echo Server: " + content;
        server.Send(res, addr);
      });
  server.Start();
  return 0;
}
