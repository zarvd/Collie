#ifndef COLLIE_LOG_STREAM_H_
#define COLLIE_LOG_STREAM_H_

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

  LogStream& operator<<(const char msg[]) noexcept {
    content_ += msg;
    return *this;
  }

  LogStream& operator<<(char* msg) noexcept {
    content_ += msg;
    return *this;
  }

  template <typename T>
  LogStream& operator<<(const T& msg) noexcept {
    content_ += std::to_string(msg);
    return *this;
  }

 private:
  const LogLevel level_;
  const std::string file_;
  const std::string func_;
  const unsigned line_;
  std::string content_;
};
}

#endif /* COLLIE_LOG_STREAM_H_ */
