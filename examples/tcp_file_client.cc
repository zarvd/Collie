#include "../include/tcp/tcp_client.h"
#include "../include/tcp/tcp_socket.h"
#include "../include/exception.h"
#include "../include/logging.h"
#include "../include/utils/file.h"

using collie::tcp::TCPClient;
using collie::tcp::TCPSocket;
using collie::utils::File;
using namespace collie;

int main(int argc, char* argv[]) {
  auto& logging = logger::LogHandler::GetHandler();
  logging.set_log_level(TRACE);
  logging.Init();

  unsigned port = 8080;
  if (argc == 2) port = std::stoul(argv[1]);

  TCPClient client;
  client.set_connect_callback([](std::shared_ptr<TCPSocket> socket) {

    REQUIRE(socket);
    socket->Send("Hello");
    File file("/home/gallon/collie/recv/test.xls", File::Mode::Write,
              File::Creat);
    const size_t size = 110080;
    Log(INFO) << "File size = " << size;
    Log(INFO) << "Receiving file...";

    socket->RecvFile(file, size);
  });
  client.Connect("127.0.0.1", port);

  return 0;
}
