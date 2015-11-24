#include "../../include/event/channel.h"
#include "../../include/descriptor.h"
#include "../../include/event/eventloop.h"
#include "../../include/poll/epoll_poller.h"
#include "../../include/exception.h"
#include "../../include/logging.h"
#include <unistd.h>

namespace collie {
namespace event {

Channel::Channel(std::shared_ptr<Descriptor> descriptor)
    : in_eventloop_(false),
      descriptor_(descriptor),
      events_(0),
      update_after_activate_(false) {
  REQUIRE(descriptor);

  Log(TRACE) << "Channel " << descriptor->Get() << " constructing";

  // default error and close callback
  close_callback_ = [this]() {
    // TODO remove channel
    Log(TRACE) << "Close channel " << descriptor_->Get();
    Remove();
  };
  error_callback_ = [this]() {  // TODO remove channel
    Log(TRACE) << "Channel " << descriptor_->Get() << " meets ERROR";
    Remove();
  };
}

Channel::~Channel() {
  Log(TRACE) << "Channel " << descriptor_->Get() << " destructing";
}

std::shared_ptr<Channel> Channel::GetCopyWithoutEventLoop() const {
  auto channel = std::make_shared<Channel>(descriptor_);
  channel->events_ = events_;
  channel->read_callback_ = read_callback_;
  channel->write_callback_ = write_callback_;
  channel->error_callback_ = error_callback_;
  channel->close_callback_ = close_callback_;
  channel->after_set_loop_callback_ = after_set_loop_callback_;
  return channel;
}

void Channel::set_eventloop(std::shared_ptr<EventLoop> eventloop) {
  REQUIRE_(!in_eventloop_, "Channel" + std::to_string(descriptor_->Get()) +
                             " is already in eventLoop");
  this->eventloop_ = eventloop;
  in_eventloop_ = true;
  if (after_set_loop_callback_) after_set_loop_callback_(shared_from_this());
}

bool Channel::IsRead() const {
  REQUIRE_(in_eventloop_, "Channel is not in event loop");
  TRACE_LOG;
  return eventloop_->kPoller->IsRead(events_);
}

bool Channel::IsWrite() const {
  REQUIRE_(in_eventloop_, "Channel is not in event loop");
  TRACE_LOG;
  return eventloop_->kPoller->IsWrite(events_);
}

void Channel::EnableRead() {
  REQUIRE_(in_eventloop_, "Channel is not in event loop");
  TRACE_LOG;
  eventloop_->kPoller->EnableRead(events_);
  Update();
}

void Channel::DisableRead() {
  REQUIRE_(in_eventloop_, "Channel is not in event loop");
  TRACE_LOG;
  eventloop_->kPoller->DisableRead(events_);
  Update();
}

void Channel::EnableWrite() {
  REQUIRE_(in_eventloop_, "Channel is not in event loop");
  TRACE_LOG;
  eventloop_->kPoller->EnableWrite(events_);
  Update();
}

void Channel::DisableWrite() {
  REQUIRE_(in_eventloop_, "Channel is not in event loop");
  TRACE_LOG;
  eventloop_->kPoller->DisableWrite(events_);
  Update();
}

void Channel::EnableOneShot() {
  REQUIRE_(in_eventloop_, "Channel is not in event loop");
  TRACE_LOG;
  eventloop_->kPoller->EnableOneShot(events_);
  Update();
}

void Channel::DisableOneShot() {
  REQUIRE_(in_eventloop_, "Channel is not in event loop");
  TRACE_LOG;
  eventloop_->kPoller->DisableOneShot(events_);
  Update();
}

void Channel::DisableAll() {
  // FIXME
  TRACE_LOG;
  events_ = 0;
  Update();
}

void Channel::activate(const unsigned revents) {
  REQUIRE(eventloop_ && eventloop_->kPoller);
  if (eventloop_->kPoller->IsError(revents)) {
    // error event
    Log(TRACE) << "Activate ERROR callback with events " << revents;
    REQUIRE_(error_callback_, "errorCallback is not callable");
    error_callback_();
  } else if (eventloop_->kPoller->IsClose(revents)) {
    // close event
    Log(TRACE) << "Activate CLOSE callback with events" << revents;
    REQUIRE_(close_callback_, "closeCallback is not callable");
    close_callback_();
  } else {
    // read event
    if (eventloop_->kPoller->IsRead(revents)) {
      if (IsRead()) {
        Log(TRACE) << "Activate READ callback with events " << revents;
        REQUIRE_(read_callback_, "readCallback is not callable");
        read_callback_();
      } else {
        Log(WARN) << "READ callback is not available";
      }
    }
    // write event
    if (eventloop_->kPoller->IsWrite(revents)) {
      if (IsWrite()) {
        Log(TRACE) << "Activate WRITE callback with events" << revents;
        REQUIRE_(write_callback_, "writeCallback is not callable");
        write_callback_();
      } else {
        Log(WARN) << "WRITE callback is not available";
      }
    }
  }
  if (update_after_activate_) Update();
}

void Channel::Update() {
  TRACE_LOG;
  eventloop_->UpdateChannel(shared_from_this());
}

void Channel::Remove() {
  TRACE_LOG;
  if (in_eventloop_) eventloop_->RemoveChannel(shared_from_this());
}
}
}
