#include "../inc/base/Logger.h"
#include "../inc/tcp/TCPClient.h"
#include "../inc/tcp/TCPStream.h"

using namespace collie;
using namespace collie::tcp;

int main(void) {
  Logger::Init();

  TCPClient::Connect("127.0.0.1", 8080, [](TCPStream& stream) {
    stream.Write("Hello,world");
    stream.Write("Hello,world");
    stream.Write("Hello,world");
    stream.Write("Hello,world\n\nHello,world");
    auto msg = stream.Read();
    LOG(INFO) << "Read: " << msg.length() << msg << " From "
              << stream.PeerAddress()->ToString();
  });

  return 0;
}
