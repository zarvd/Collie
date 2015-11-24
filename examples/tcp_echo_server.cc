#include "../include/tcp/tcp_server.h"
#include "../include/tcp/tcp_connection.h"
#include "../include/inet_address.h"
#include "../include/exception.h"
#include "../include/logging.h"

using collie::tcp::TCPServer;
using collie::tcp::TCPConnection;
using namespace collie;

int main(int argc, char* argv[]) {
  auto& logging = logger::LogHandler::GetHandler();
  logging.set_log_level(TRACE);
  logging.Init();

  unsigned port = 8080;
  if (argc == 2) port = std::stoul(argv[1]);

  TCPServer server;
  server.Bind("0.0.0.0", port);
  server.set_on_message_callback([](std::shared_ptr<TCPConnection> conn) {
    REQUIRE(conn);
    const auto content = conn->RecvAll();
    auto addr = conn->GetRemoteAddress();
    REQUIRE(addr);
    Log(INFO) << addr->ip() << " (" << addr->port() << "): " << content;
    conn->Send("Hi, here is server");
  });
  server.Start();
  return 0;
}
