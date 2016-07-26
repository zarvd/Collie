#include "../inc/base/EventThreadPool.h"
#include "../inc/base/Logger.h"
#include "../inc/tcp/AsyncTCPServer.h"
#include "../inc/tcp/AsyncTCPStream.h"

using namespace collie;
using namespace collie::tcp;

int main(void) {
  Logger::SetLevel(DEBUG);
  Logger::Init();
  AsyncTCPServer server;

  using IOStream = std::shared_ptr<AsyncTCPStream>;

  auto pool = std::make_shared<EventThreadPool>(1);
  server.SetEventLoop(pool);

  server.SetRequestHandler([](IOStream stream) {
    // stream->Write("Hello, world", nullptr);
    stream->Read([](IOStream stream) {
      LOG(DEBUG) << "recv: " << stream->ReadBuffer();
      stream->Write(stream->ReadBuffer(), nullptr);
    });
  });
  server.Listen(8080);
  pool->Start();

  return 0;
}
