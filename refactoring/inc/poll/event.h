#ifndef COLLIE_POLL_EVENT_H_
#define COLLIE_POLL_EVENT_H_

#include "../util/noncopyable.h"
#include "../util/descriptor.h"

namespace collie {

class Event : public NonCopyable, public Descriptor {
 public:
  enum Type : unsigned { Read, Write };

  Event();
  virtual ~Event() = 0;
  virtual int GetDescriptor() const noexcept override = 0;
  virtual void HandleRead() = 0;
  virtual void HandleWrite() = 0;
  virtual void HandleClose() = 0;
  virtual void HandleError() = 0;
};
}

#endif /* COLLIE_POLL_EVENT_H_ */
