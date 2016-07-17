#include "../../inc/base/Logger.h"

namespace collie {

LogStream::LogStream(LogLevel level, const String& file, const String& func,
                     unsigned line) noexcept : level(level),
                                               file(file),
                                               func(func),
                                               line(line) {}

LogStream::~LogStream() noexcept {
  Logger::Log(level, content, file, func, line);
}
}
