#include "../inc/logger.h"

namespace collie {
std::vector<std::unique_ptr<LogHandler> > Logger::handlers_;

std::mutex Logger::handlers_mutex_;

std::atomic<LogLevel> Logger::log_level_(INFO);

void Logger::AddLogHandler(std::unique_ptr<LogHandler> log_handler) noexcept {
  std::lock_guard<std::mutex> lock(Logger::handlers_mutex_);
  handlers_.push_back(std::move(log_handler));
}

void Logger::ClearLogHandler() noexcept {
  std::lock_guard<std::mutex> lock(Logger::handlers_mutex_);
  handlers_.clear();
}
}
