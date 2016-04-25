#ifndef COLLIE_LOG_LIB_H
#define COLLIE_LOG_LIB_H

#include <string>

namespace collie {

const std::string LOG_COLOR_HEADER = "\033[95m";
const std::string LOG_COLOR_OKBLUE = "\033[94m";
const std::string LOG_COLOR_OKGREEN = "\033[92m";
const std::string LOG_COLOR_WARNING = "\033[93m";
const std::string LOG_COLOR_FAIL = "\033[91m";
const std::string LOG_COLOR_ENDC = "\033[0m";
const std::string LOG_COLOR_BOLD = "\033[1m";
const std::string LOG_COLOR_UNDERLINE = "\033[4m";

enum LogLevel { DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4 };

// Log Level to string with console color
inline std::string LogLevelToString(LogLevel level, bool with_color = true) {
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
}
#endif /* COLLIE_LOG_LIB_H */
