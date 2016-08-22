#include <collie/LogHandler.h>
#include <ctime>
#include <iostream>

namespace collie {

constexpr char LOG_COLOR_HEADER[] = "\033[95m";
constexpr char LOG_COLOR_OKBLUE[] = "\033[94m";
constexpr char LOG_COLOR_OKGREEN[] = "\033[92m";
constexpr char LOG_COLOR_WARNING[] = "\033[93m";
constexpr char LOG_COLOR_FAIL[] = "\033[91m";
constexpr char LOG_COLOR_ENDC[] = "\033[0m";
constexpr char LOG_COLOR_BOLD[] = "\033[1m";
// constexpr char LOG_COLOR_UNDERLINE[] = "\033[4m";

// Log Level to string with console color
inline std::string ToString(LogLevel level, bool with_color = true) {
  std::string color_begin;
  std::string level_str;
  std::string color_end = LOG_COLOR_ENDC;
  switch (level) {
    case DEBUG:
      color_begin = LOG_COLOR_OKBLUE;
      level_str = "DEBUG";
      break;
    case INFO:
      color_begin = LOG_COLOR_OKGREEN;
      level_str = "INFO";
      break;
    case WARN:
      color_begin = LOG_COLOR_WARNING;
      level_str = "WARN";
      break;
    case ERROR:
      color_begin = LOG_COLOR_FAIL;
      level_str = "ERROR";
      break;
  }
  if (with_color)
    return color_begin + level_str + color_end;
  else
    return level_str;
}

void LogHandler::Log(const LogLevel level, const std::string& msg,
                     const std::string& file, const std::string&,
                     unsigned int line) const noexcept {
  ::time_t now;
  ::time(&now);
  char buffer[80];
  ::strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", localtime(&now));

  std::cout << std::endl
            << LOG_COLOR_HEADER << buffer << LOG_COLOR_ENDC << std::endl
            << '[' << ToString(level) << ']' << file << "(" << line
            << "): " << LOG_COLOR_BOLD << msg << LOG_COLOR_ENDC << std::endl;
}
}
