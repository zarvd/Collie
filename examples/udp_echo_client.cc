#include "../include/udp/udp_socket.h"
#include "../include/udp/udp_client.h"
#include "../include/inet_address.h"
#include "../include/exception.h"
#include "../include/logging.h"

using collie::udp::UDPClient;
using namespace collie;

int main(int argc, char* argv[]) {
  auto& logging = logger::LogHandler::GetHandler();
  logging.set_log_level(TRACE);
  logging.Init();

  unsigned port = 8080;
  if (argc == 2) port = std::stoul(argv[1]);

  UDPClient client;
  client.set_connect_callback(
      [&client](const std::string& content, std::shared_ptr<InetAddress> addr) {
        Log(INFO) << content;
        const auto res = "From UDP Echo Client: " + content;
        client.Send(res, addr);
        return false;
      });
  client.Connect("127.0.0.1", port, "Hello world");
  return 0;
}
