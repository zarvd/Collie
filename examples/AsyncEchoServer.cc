#include "../inc/base/EventThreadPool.h"
#include "../inc/base/Logger.h"
#include "../inc/tcp/AsyncTCPServer.h"
#include "../inc/tcp/AsyncTCPStream.h"

using namespace collie;
using namespace collie::tcp;

int main(void) {
  Logger::SetLevel(DEBUG);
  Logger::Init();

  using IOStream = std::shared_ptr<AsyncTCPStream>;

  auto pool = std::make_shared<EventThreadPool>(1);
  AsyncTCPServer server(pool);

  server.SetRequestHandler([](IOStream stream) {
    // stream->Write("Hello, world", nullptr);
    stream->Read([](IOStream stream) {
      LOG(DEBUG) << "Read: " << stream->ReadBuffer() << " From "
                 << stream->PeerAddress()->ToString();
      stream->Write(stream->ReadBuffer(), nullptr);
    });
  });
  pool->Start();
  server.Listen(8081).Start();

  return 0;
}
