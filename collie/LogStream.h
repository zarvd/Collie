#pragma once

#include <collie/LogLib.h>
#include <collie/Serializable.h>
#include <collie/String.h>
#include <collie/NonCopyable.h>

namespace collie {

// Does logging when destructing
// Uses stream style input
class LogStream : public NonCopyable {
 public:
  LogStream(LogLevel level, const std::string& file, const std::string& func,
            unsigned line) noexcept;
  ~LogStream() noexcept;

  LogStream& operator<<(const std::string& msg) noexcept {
    content += msg;
    return *this;
  }

  LogStream& operator<<(const char* msg) noexcept {
    content += msg;
    return *this;
  }

  LogStream& operator<<(char msg[]) noexcept {
    content += msg;
    return *this;
  }

  LogStream& operator<<(const Serializable& msg) noexcept {
    content += msg.ToString();
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
