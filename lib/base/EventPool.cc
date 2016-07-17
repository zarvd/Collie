#include "../../inc/base/EventPool.h"
#include "../../inc/base/AsyncIOStream.h"
#include "../../inc/base/EventType.h"
#include "../../inc/base/Logger.h"

namespace collie {
namespace base {

EventPool::EventPool() noexcept {}

EventPool::~EventPool() noexcept {}

void EventPool::Init() noexcept {
  poller = std::make_unique<POLLER>();
  try {
    poller->Init();
  } catch (std::system_error) {
  }
}

void EventPool::Destroy() noexcept {
  try {
    poller->Destroy();
  } catch (std::system_error) {
  }
}

void EventPool::Loop(int timeout) noexcept {
  while (true) {
    LoopOne(timeout);
  }
}

void EventPool::LoopOne(int timeout) noexcept {
  using namespace std::placeholders;
  poller->Poll(std::bind(&EventPool::PollHandler, this, _1, _2), timeout);
}

void EventPool::Update(std::shared_ptr<AsyncIOStream> stream) noexcept {
  if (io_streams.find(stream->Descriptor()) == io_streams.end()) {
    // new io
    io_streams.insert({stream->Descriptor(), stream});
    poller->Insert(stream->Descriptor(), stream->Event());  // throwable
    stream->event_pool = shared_from_this();
  } else {
    poller->Update(stream->Descriptor(), stream->Event());
  }
}

void EventPool::Delete(std::shared_ptr<AsyncIOStream> stream) noexcept {
  auto it = io_streams.find(stream->Descriptor());

  if (it == io_streams.end()) {
    LOG(WARN) << "IO stream is not in event pool";
    return;
  }
  poller->Delete(stream->Descriptor());

  stream->event_pool = nullptr;
  io_streams.erase(it);
}

void EventPool::PollHandler(unsigned int fd, EventType revents) noexcept {
  auto it = io_streams.find(fd);

  if (it == io_streams.end()) {
    LOG(WARN) << "IO stream is not in event pool";
    return;
  }
  auto io = it->second;
  if (revents.IsRead()) {
    LOG(DEBUG) << "read events";
    io->HandleRead();
  }
  if (revents.IsWrite()) {
    LOG(DEBUG) << "write events";
    io->HandleWrite();
  }
  if (revents.IsError()) {
    LOG(DEBUG) << "error events";
    io->HandleError();
  }
  if (revents.IsClose()) {
    LOG(DEBUG) << "close events";
    io->HandleClose();
  }
}
}
}
