#ifndef COLLIE_BASE_LOG_HANDLER_H_
#define COLLIE_BASE_LOG_HANDLER_H_

#include "../util/NonCopyable.h"
#include "LogLib.h"
#include "String.h"

namespace collie {
namespace base {

// Default log handler
class LogHandler : public util::NonCopyable {
 public:
  LogHandler() noexcept {}
  virtual ~LogHandler() noexcept {}

  virtual void Log(const LogLevel, const String& msg, const String& file,
                   const String& func, unsigned line) const noexcept;
};
}
}

#endif /* COLLIE_BASE_LOG_HANDLER_H_ */
