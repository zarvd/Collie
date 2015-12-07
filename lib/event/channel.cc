#include "../../include/event/channel.h"
#include "../../include/descriptor.h"
#include "../../include/event/eventloop.h"
#include "../../include/poll/epoll_poller.h"
#include "../../include/logging.h"
#include <unistd.h>

namespace collie {
namespace event {

Channel::Channel(std::shared_ptr<Descriptor> descriptor)
    : SurvivalTime("Channel " + std::to_string(descriptor->fd())),
      in_eventloop_(false),
      descriptor_(descriptor),
      events_(0),
      update_after_activate_(false) {
  CHECK(descriptor);

  // default error and close callback
  close_callback_ = [](auto channel) {
    // TODO remove channel
    channel->Remove();
  };
  error_callback_ = [](auto channel) {  // TODO remove channel
    channel->Remove();
  };
}

Channel::~Channel() {}

// get a copy share_ptr of channel without coping event loop
std::shared_ptr<Channel> Channel::GetCopyWithoutEventLoop() const {
  auto channel = std::make_shared<Channel>(descriptor_);
  channel->events_ = events_;
  channel->read_callback_ = read_callback_;
  channel->write_callback_ = write_callback_;
  channel->error_callback_ = error_callback_;
  channel->close_callback_ = close_callback_;
  channel->insert_callback_ = insert_callback_;
  return channel;
}

bool Channel::IsRead() const {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  return eventloop_->kPoller->IsRead(events_);
}

bool Channel::IsWrite() const {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  return eventloop_->kPoller->IsWrite(events_);
}

void Channel::EnableRead() {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  eventloop_->kPoller->EnableRead(events_);
  Update();
}

void Channel::DisableRead() {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  eventloop_->kPoller->DisableRead(events_);
  Update();
}

void Channel::EnableWrite() {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  eventloop_->kPoller->EnableWrite(events_);
  Update();
}

void Channel::DisableWrite() {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  eventloop_->kPoller->DisableWrite(events_);
  Update();
}

void Channel::EnableOneShot() {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  eventloop_->kPoller->EnableOneShot(events_);
  Update();
}

void Channel::DisableOneShot() {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  eventloop_->kPoller->DisableOneShot(events_);
  Update();
}

void Channel::DisableAll() {
  // FIXME
  DisableRead();
  DisableWrite();
}

void Channel::Activate(const unsigned revents) {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  LOG(DEBUG) << "Channel" << descriptor_->fd() << "is activating events " << revents;
  if (eventloop_->kPoller->IsError(revents)) {
    // error event
    CHECK(error_callback_) << "errorCallback is not callable";
    error_callback_(shared_from_this());
  } else if (eventloop_->kPoller->IsClose(revents)) {
    // close event
    CHECK(close_callback_) << "closeCallback is not callable";
    close_callback_(shared_from_this());
  } else {
    // read event
    if (eventloop_->kPoller->IsRead(revents)) {
      if (IsRead()) {
        CHECK(read_callback_) << "readCallback is not callable";
        read_callback_(shared_from_this());
      } else {
        LOG(WARNING) << "READ callback is not available";
      }
    }
    // write event
    if (eventloop_->kPoller->IsWrite(revents)) {
      if (IsWrite()) {
        CHECK(write_callback_) << "writeCallback is not callable";
        write_callback_(shared_from_this());
      } else {
        LOG(WARNING) << "WRITE callback is not available";
      }
    }
    if (update_after_activate_) Update();
  }
}

void Channel::Update() {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  eventloop_->UpdateChannel(shared_from_this());
}

void Channel::Remove() {
  CHECK(in_eventloop_) << "Channel is not in event loop";
  eventloop_->RemoveChannel(shared_from_this());
  in_eventloop_ = false;
}
}
}
