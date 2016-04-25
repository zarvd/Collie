#include <iostream>
#include <ctime>
#include "../inc/logger.h"

namespace collie {

void Logger::Log(const collie::LogLevel level, const std::string &msg,
                 const std::string &file, const std::string &,
                 unsigned int) noexcept {
  ::time_t now;
  ::time(&now);
  char buffer[80];
  ::strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", localtime(&now));

  std::cout << std::endl
            << LOG_COLOR_HEADER << buffer << LOG_COLOR_ENDC << std::endl
            << '[' << LogLevelToString(level) << ']' << file << LOG_COLOR_BOLD
            << msg << LOG_COLOR_ENDC << std::endl;
}

LogStream::LogStream(LogLevel level, const std::string &file,
                     const std::string &func, unsigned line) noexcept
    : level_(level),
      file_(file),
      func_(func),
      line_(line) {}
}
