#include "../inc/base/Logger.h"
#include "../inc/tcp/TCPServer.h"
#include "../inc/tcp/TCPStream.h"

using namespace collie;
using namespace collie::tcp;

int main(void) {
  Logger::Init();

  TCPServer server;

  server.SetRequestHandler([](TCPStream& tcp_stream) {
    auto greeting = tcp_stream.Read();
    LOG(INFO) << "Read: " << greeting << " From "
              << tcp_stream.PeerAddress()->ToString();
    tcp_stream.Write(greeting);
    tcp_stream.Abort();
  });
  server.Listen(8080).Start();

  return 0;
}
