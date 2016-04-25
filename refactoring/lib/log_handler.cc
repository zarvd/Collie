#include <iostream>
#include <ctime>
#include "../inc/log_handler.h"

namespace collie {

void LogHandler::Log(const collie::LogLevel level, const std::string &msg,
                     const std::string &file, const std::string &,
                     unsigned int line) const noexcept {
  ::time_t now;
  ::time(&now);
  char buffer[80];
  ::strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", localtime(&now));

  std::cout << std::endl
            << LOG_COLOR_HEADER << buffer << LOG_COLOR_ENDC << std::endl
            << '[' << LogLevelToString(level) << ']' << file << "(" << line
            << "): " << LOG_COLOR_BOLD << msg << LOG_COLOR_ENDC << std::endl;
}
}
