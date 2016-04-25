#ifndef COLLIE_LOG_STREAM_H
#define COLLIE_LOG_STREAM_H

#include <vector>
#include <memory>
#include <mutex>
#include "util/noncopyable.h"
#include "loglib.h"

namespace collie {

class LogHandler;

// Does logging when destructing
// Uses stream style input
class LogStream : public NonCopyable {
 public:
  LogStream(LogLevel, const std::string& file, const std::string& func,
            unsigned line) noexcept;
  ~LogStream() noexcept;

  LogStream& operator<<(const std::string& msg) noexcept {
    content_ += msg;
    return *this;
  }

  template <typename T = char>
  LogStream& operator<<(const T msg[]) noexcept {
    content_ += msg;
    return *this;
  }

  template <typename T>
  LogStream& operator<<(const T& msg) noexcept {
    content_ += std::to_string(msg);
    return *this;
  }

  // Thread safe
  static void AddLogHandler(std::unique_ptr<LogHandler>) noexcept;
  static void ClearLogHandler() noexcept;
  static unsigned GetLoggerNum() noexcept { return handlers_.size(); }

 private:
  const LogLevel level_;
  const std::string file_;
  const std::string func_;
  const unsigned line_;
  static std::vector<std::unique_ptr<LogHandler> > handlers_;
  static std::mutex handlers_mutex_;

  std::string content_;
};
}

#endif /* COLLIE_LOG_STREAM_H */
