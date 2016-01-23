#include "../include/tcp/tcp_server.h"
#include "../include/tcp/tcp_iostream.h"
#include "../include/inet_address.h"
#include "../include/logging.h"

using namespace collie;
using tcp::TCPServer;
using tcp::TCPIOStream;

// initialize logging
INITIALIZE_EASYLOGGINGPP;

int main(int argc, char* argv[]) {
  ConfigureLogger("/home/gallon/collie/logging.conf");
  unsigned port = 8080;
  if (argc == 2) port = std::stoul(argv[1]);

  TCPServer server;
  server.Bind("0.0.0.0", port);
  server.set_on_message_callback([](auto iostream) {
    // LOG(INFO) << peer_address->ip() << " (" << peer_address->port() << "): "
    //           << "Connect";

    iostream->ReadUntil("\n", [](const std::string& msg,
                                 std::shared_ptr<TCPIOStream> iostream) {
      iostream->Write("Hello, " + msg);
    });

  });
  server.Start();
  return 0;
}
