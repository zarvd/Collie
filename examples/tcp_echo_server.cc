#include "../include/tcp/tcp_server.h"
#include "../include/tcp/tcp_iostream.h"
#include "../include/inet_address.h"
#include "../include/exception.h"
#include "../include/logging.h"

using collie::tcp::TCPServer;
using collie::tcp::TCPIOStream;
using collie::InetAddress;
using namespace collie;

int main(int argc, char* argv[]) {
  auto& logging = logger::LogHandler::GetHandler();
  logging.set_log_level(TRACE);
  logging.Init();

  unsigned port = 8080;
  if (argc == 2) port = std::stoul(argv[1]);

  TCPServer server;
  server.Bind("0.0.0.0", port);
  server.set_on_message_callback([](std::shared_ptr<TCPIOStream> iostream,
                                    std::shared_ptr<InetAddress>,
                                    std::shared_ptr<InetAddress> peer_address) {
    Log(INFO) << peer_address->ip() << " (" << peer_address->port() << "): "
              << "Connect";

    iostream->ReadUntil("\n", [](const std::string& msg,
                                 std::shared_ptr<TCPIOStream> iostream) {
      iostream->Write("Hello, " + msg);
    });

  });
  server.Start();
  return 0;
}
