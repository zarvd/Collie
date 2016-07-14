#ifndef COLLIE_BASE_LOG_STREAM_H_
#define COLLIE_BASE_LOG_STREAM_H_

#include <memory>
#include <mutex>
#include <vector>
#include "../util/noncopyable.h"
#include "loglib.h"

namespace collie {
namespace base {

class LogHandler;

// Does logging when destructing
// Uses stream style input
class LogStream : public util::NonCopyable {
 public:
  LogStream(LogLevel, const std::string& file, const std::string& func,
            unsigned line) noexcept;
  ~LogStream() noexcept;

  LogStream& operator<<(const std::string& msg) noexcept {
    content += msg;
    return *this;
  }

  LogStream& operator<<(const char msg[]) noexcept {
    content += msg;
    return *this;
  }

  LogStream& operator<<(char* msg) noexcept {
    content += msg;
    return *this;
  }

  template <typename T>
  LogStream& operator<<(const T& msg) noexcept {
    content += std::to_string(msg);
    return *this;
  }

 private:
  const LogLevel level;
  const std::string file;
  const std::string func;
  const unsigned line;
  std::string content;
};
}
}

#endif /* COLLIE_BASE_LOG_STREAM_H_ */
