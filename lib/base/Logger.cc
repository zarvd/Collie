#include "../../inc/base/Logger.h"

namespace collie {

Logger Logger::logger;

Logger::~Logger() { ClearLogHandler(); }

void Logger::Init() { logger.AddLogHandler<LogHandler>(); }

void Logger::SetLevel(LogLevel level) noexcept { logger.log_level = level; }

LogLevel Logger::Level() noexcept { return logger.log_level; }

SizeType Logger::NumOfHandler() noexcept { return logger.handlers.size(); }

void Logger::Log(const LogLevel level, const std::string& msg,
                 const std::string& file, const std::string& func,
                 unsigned line) noexcept {
  std::lock_guard<std::mutex> lock(logger.handlers_mutex);
  for (auto& handler : logger.handlers) {
    handler->Log(level, msg, file, func, line);
  }
}

void Logger::ClearLogHandler() noexcept {
  std::lock_guard<std::mutex> lock(logger.handlers_mutex);
  for (auto it = logger.handlers.begin(); it != logger.handlers.end(); ++it) {
    delete *it;
  }
  logger.handlers.clear();
}
}
