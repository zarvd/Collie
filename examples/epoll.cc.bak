#include "../inc/poll/epoll_poller.h"
#include "../inc/logger.h"

using namespace collie;

int main(void) {
  Logger::Init();
  EPollPoller poller;
  LOG(INFO) << poller.GetDescriptor();
  poller.Init();
  LOG(INFO) << poller.GetDescriptor();
  poller.Destroy();
  LOG(INFO) << poller.GetDescriptor();
  poller.Destroy();
  return 0;
}
