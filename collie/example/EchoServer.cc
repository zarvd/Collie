#include "../inc/base/Logger.h"
#include "../inc/tcp/TCPServer.h"
#include "../inc/tcp/TCPStream.h"

using namespace collie;
using namespace collie::tcp;

int main(void) {
  Logger::Init();

  TCPServer server;

  server.SetRequestHandler([](TCPStream& stream) {
    auto greeting = stream.ReadUntil("\n\n");
    LOG(INFO) << "Read: " << greeting << " From "
              << stream.PeerAddress()->ToString();
    stream.Write(greeting);
  });
  server.Listen(8080).Start();

  return 0;
}
