#ifndef COLLIE_LOGGER_H_
#define COLLIE_LOGGER_H_

#include "util/noncopyable.h"
#include <string>

namespace collie {
enum LogLevel { DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4 };

inline std::string LogLevelToString(LogLevel level) {
  switch (level) {
    case DEBUG:
      return "DEBUG";
    case INFO:
      return "INFO";
    case WARN:
      return "WARN";
    case ERROR:
      return "ERROR";
  }
}

// Singleton
class Logger : public NonCopyable {
 public:
  virtual ~Logger() noexcept {}

  static Logger& GetInstance() noexcept {
    static Logger instance;
    return instance;
  }

  virtual void Log(const LogLevel, const std::string& msg,
                   const std::string& file, const std::string& func,
                   unsigned line) noexcept;

  static LogLevel level() noexcept { return GetInstance().level_; }
  static void set_level(LogLevel level) noexcept {
    GetInstance().level_ = level;
  }

 private:
  Logger() noexcept : level_(INFO) {}
  LogLevel level_;
};

class LogStream : public NonCopyable {
 public:
  LogStream(LogLevel, const std::string& file, const std::string& func,
            unsigned line) noexcept;
  ~LogStream() noexcept {
    Logger::GetInstance().Log(level_, content_, file_, func_, line_);
  }

  LogStream& operator<<(const std::string& msg) noexcept {
    content_ += msg;
    return *this;
  }

  template <typename T = char>
  LogStream& operator<<(T msg[]) noexcept {
    content_ += msg;
    return *this;
  }

  template <typename T>
  LogStream& operator<<(const T& msg) noexcept {
    content_ += std::to_string(msg);
    return *this;
  }

 private:
  const LogLevel level_;
  const std::string file_;
  const std::string func_;
  const unsigned line_;

  std::string content_;
};

constexpr int32_t basename_index(const char* const path,
                                 const int32_t index = 0,
                                 const int32_t slash_index = -1) {
  return path[index] ? (path[index] == '/'
                            ? basename_index(path, index + 1, index)
                            : basename_index(path, index + 1, slash_index))
                     : (slash_index + 1);
}

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define __FILENAME__                                              \
  ({                                                              \
    static const int32_t basename_idx = basename_index(__FILE__); \
    static_assert(basename_idx >= 0, "compile-time basename");    \
    __FILE__ ":" STRINGIZE(__LINE__) ": " + basename_idx;         \
  })

#define LOG(log_level)             \
  if (log_level < Logger::level()) \
    ;                              \
  else                             \
  LogStream(log_level, __FILENAME__, __func__, __LINE__)

#define ASSERT(x)                   \
  if (!(x)) {                       \
    LOG(ERROR) << "ASSERTION FAIL"; \
    throw;                          \
  }
}

#endif /* COLLIE_LOGGER_H_ */
