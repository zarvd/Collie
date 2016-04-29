#include "../../inc/poll/event.h"
#include "../../inc/poll/event_pool.h"
#include "../../inc/poll/async_io_stream.h"
#include "../../inc/logger.h"

namespace collie {

EventPool::EventPool() noexcept {}

EventPool::~EventPool() noexcept {}

void EventPool::Init() throw(PollException) {
  poller_ = std::make_unique<POLLER>();
  poller_->Init();
}

void EventPool::Destroy() throw(PollException) { poller_->Destroy(); }

void EventPool::Loop(int timeout) throw(PollException) {
  while (true) {
    LoopOne(timeout);
  }
}

void EventPool::LoopOne(int timeout) throw(PollException) {
  using namespace std::placeholders;
  poller_->Poll(std::bind(&EventPool::PollHandler, this, _1, _2), timeout);
}

void EventPool::Update(std::shared_ptr<AsyncIOStream> stream) throw(
    PollException) {
  if (io_streams_.find(stream->GetDescriptor()) != io_streams_.end()) {
    // new io
    io_streams_.insert({stream->GetDescriptor(), stream});
    poller_->Insert(stream->GetDescriptor(), stream->event());
    stream->event_pool_ = shared_from_this();
  } else {
    poller_->Update(stream->GetDescriptor(), stream->event());
  }
}

void EventPool::Delete(std::shared_ptr<AsyncIOStream> stream) throw(
    PollException) {
  auto it = io_streams_.find(stream->GetDescriptor());

  if (it == io_streams_.end()) {
    LOG(WARN) << "IO stream is not in event pool";
    return;
  }

  stream->event_pool_ = nullptr;
  io_streams_.erase(it);
}
}
