#pragma once

#include <collie/LogLib.h>
#include <collie/String.h>
#include <collie/NonCopyable.h>

namespace collie {

// Default log handler
class LogHandler : public NonCopyable {
 public:
  LogHandler() noexcept {}
  virtual ~LogHandler() noexcept {}

  virtual void Log(const LogLevel, const std::string& msg,
                   const std::string& file, const std::string& func,
                   unsigned line) const noexcept;
};
}
