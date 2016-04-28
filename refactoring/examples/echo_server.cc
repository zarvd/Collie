#include "../inc/tcp_server.h"
#include "../inc/tcp_stream.h"
#include "../inc/logger.h"

using namespace collie;

int main(void) {
  Logger::Init();

  TcpServer server;

  // try {
  server.SetRequestHandler([](TcpStream& tcp_stream) {
          auto greeting = tcp_stream.Read();
          LOG(INFO) << "Read: " << greeting;
          tcp_stream.Write(greeting);
          tcp_stream.Abort();
        })
      .Listen(8080)
      .Start();
  // } catch (TcpException& e) {
  //   LOG(WARN) << e.what();
  // }
  return 0;
}
