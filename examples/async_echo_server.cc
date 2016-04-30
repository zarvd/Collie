#include "../inc/poll/event_thread_pool.h"
#include "../inc/async_tcp_server.h"
#include "../inc/async_tcp_stream.h"
#include "../inc/logger.h"

using namespace collie;

int main(void) {
  Logger::SetLogLevel(DEBUG);
  Logger::Init();
  AsyncTcpServer server;

  using IOStream = std::shared_ptr<AsyncTcpStream>;

  auto pool = std::make_shared<EventThreadPool>(1);

  server.SetRequestHandler([](IOStream stream) {
    // stream->Write("Hello, world", nullptr);
    stream->Read([](IOStream stream) {
      LOG(DEBUG) << "recv: " << stream->read_buffer();
      stream->Write(stream->read_buffer(), nullptr);
    });
  });

  server.Listen(8080);
  server.SetLoop(pool);
  pool->Start();

  return 0;
}
