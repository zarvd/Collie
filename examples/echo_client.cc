#include "../inc/tcp_client.h"
#include "../inc/tcp_stream.h"
#include "../inc/logger.h"

using namespace collie;

int main(void) {
  Logger::Init();
  RUN_FUNC;

  TcpClient::Connect("127.0.0.1", 8080, [](TcpStream& tcp_stream) {
    tcp_stream.Write("Hello, world!");
    auto msg = tcp_stream.Read();
    LOG(INFO) << "Read: " << msg;
  });

  return 0;
}
