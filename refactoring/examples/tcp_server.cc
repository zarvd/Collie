#include "../inc/tcp_server.h"
#include "../inc/logger.h"

using namespace collie;

int main(void) {
  Logger::Init();

  TcpServer server;

  try {
    server.SetRequestHandler([](TcpStream& tcp_stream) {
            auto greeting = tcp_stream.Read();
            tcp_stream.Write(greeting);
          })
        .Listen(8080)
        .Start();
  } catch (TcpException& e) {
    LOG(WARN) << e.what();
  }
  return 0;
}
