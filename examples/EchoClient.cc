#include "../inc/base/Logger.h"
#include "../inc/tcp/TCPClient.h"
#include "../inc/tcp/TCPStream.h"

using namespace collie;
using namespace collie::tcp;

int main(void) {
  Logger::Init();

  TCPClient::Connect("127.0.0.1", 8080, [](TCPStream& tcp_stream) {
    tcp_stream.Write("Hello, world!");
    auto msg = tcp_stream.Read();
    LOG(INFO) << "Read: " << msg << " From "
              << tcp_stream.PeerAddress()->ToString();
  });

  return 0;
}
