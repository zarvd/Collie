#include "../inc/log_stream.h"
#include "../inc/log_handler.h"

namespace collie {

std::vector<std::unique_ptr<LogHandler> > LogStream::handlers_;

std::mutex LogStream::handlers_mutex_;

LogStream::LogStream(LogLevel level, const std::string& file,
                     const std::string& func, unsigned line) noexcept
    : level_(level),
      file_(file),
      func_(func),
      line_(line) {}

LogStream::~LogStream() noexcept {
  std::lock_guard<std::mutex> lock(handlers_mutex_);
  for (auto& handler : handlers_) {
    handler->Log(level_, content_, file_, func_, line_);
  }
}

void LogStream::AddLogHandler(
    std::unique_ptr<LogHandler> log_handler) noexcept {
  std::lock_guard<std::mutex> lock(handlers_mutex_);
  handlers_.push_back(std::move(log_handler));
}

void LogStream::ClearLogHandler() noexcept {
  std::lock_guard<std::mutex> lock(handlers_mutex_);
  handlers_.clear();
}
}
