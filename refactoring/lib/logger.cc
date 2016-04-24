#include <iostream>
#include "../inc/logger.h"

namespace collie {

void Logger::Log(const collie::LogLevel level, const std::string &msg,
                 const std::string &file, const std::string &,
                 unsigned int) noexcept {
  std::cout << '['<< LogLevelToString(level) << ']' << file << msg << std::endl;
}

LogStream::LogStream(LogLevel level, const std::string &file,
                     const std::string &func, unsigned line) noexcept
    : level_(level),
      file_(file),
      func_(func),
      line_(line) {}
}
