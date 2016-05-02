#ifndef COLLIE_LOG_HANDLER_H_
#define COLLIE_LOG_HANDLER_H_

#include <atomic>
#include "util/noncopyable.h"
#include "loglib.h"

namespace collie {

// Default log handler
class LogHandler : public NonCopyable {
 public:
  LogHandler() noexcept : log_level_(INFO) {}
  virtual ~LogHandler() noexcept {}

  virtual void Log(const LogLevel, const std::string& msg,
                   const std::string& file, const std::string& func,
                   unsigned line) const noexcept;

  LogLevel level() const noexcept { return log_level_; }
  // Thread safe
  void set_level(LogLevel level) noexcept { log_level_ = level; }

 protected:
  std::atomic<LogLevel> log_level_;
};
}

#endif /* COLLIE_LOG_HANDLER_H_ */
