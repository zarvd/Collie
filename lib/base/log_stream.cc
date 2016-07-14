// #include "../inc/log_stream.h"
// #include "../inc/log_handler.h"
#include "../inc/logger.h"

namespace collie {

LogStream::LogStream(LogLevel level, const std::string& file,
                     const std::string& func, unsigned line) noexcept
    : level_(level),
      file_(file),
      func_(func),
      line_(line) {}

LogStream::~LogStream() noexcept {
  std::lock_guard<std::mutex> lock(Logger::handlers_mutex_);
  for (auto& handler : Logger::handlers_) {
    handler->Log(level_, content_, file_, func_, line_);
  }
}
}
