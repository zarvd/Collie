#ifndef COLLIE_BASE_LOG_HANDLER_H_
#define COLLIE_BASE_LOG_HANDLER_H_

#include <atomic>
#include "../util/noncopyable.h"
#include "loglib.h"

namespace collie {

// Default log handler
class LogHandler : public util::NonCopyable {
 public:
  LogHandler() noexcept : log_level(INFO) {}
  virtual ~LogHandler() noexcept {}

  virtual void Log(const LogLevel, const std::string& msg,
                   const std::string& file, const std::string& func,
                   unsigned line) const noexcept;

  LogLevel GetLevel() const noexcept { return log_level; }
  // Thread safe
  void SetLevel(LogLevel level) noexcept { log_level = level; }

 protected:
  std::atomic<LogLevel> log_level;
};
}

#endif /* COLLIE_BASE_LOG_HANDLER_H_ */
