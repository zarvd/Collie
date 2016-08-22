#include <collie/Logger.h>

namespace collie {

LogStream::LogStream(LogLevel level, const std::string& file,
                     const std::string& func, unsigned line) noexcept
    : level(level),
      file(file),
      func(func),
      line(line) {}

LogStream::~LogStream() noexcept {
  Logger::Log(level, content, file, func, line);
}
}
