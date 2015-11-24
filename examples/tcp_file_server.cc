#include "../include/tcp/tcp_server.h"
#include "../include/tcp/tcp_connection.h"
#include "../include/inet_address.h"
#include "../include/exception.h"
#include "../include/logging.h"
#include "../include/utils/file.h"

using collie::tcp::TCPServer;
using collie::tcp::TCPConnection;
using collie::utils::File;
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
    conn->RecvAll();
    File file("/home/gallon/collie/send/test.xls", File::Mode::Read);
    if (!file.is_existed() || !file.IsFile()) {
      Log(WARN) << file.kPathName << " not found";
      return;
    }
    Log(INFO) << "File size = " << file.GetSize();
    Log(INFO) << "Sending file...";
    conn->SendFile(file);
  });
  server.Start();

  return 0;
}
