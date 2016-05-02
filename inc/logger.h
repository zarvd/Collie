#ifndef COLLIE_LOGGER_H_
#define COLLIE_LOGGER_H_

#include "log_handler.h"
#include "log_stream.h"

namespace collie {

// Global logger configuration
class Logger {
 public:
  ~Logger() noexcept {}

  static void Init() { AddLogHandler(std::make_unique<LogHandler>()); }

  // Thread safe
  static void SetLogLevel(LogLevel level) noexcept { log_level_ = level; }
  static LogLevel GetLogLevel() noexcept { return log_level_; }
  static void AddLogHandler(std::unique_ptr<LogHandler>) noexcept;
  static void ClearLogHandler() noexcept;
  static unsigned GetLoggerNum() noexcept { return handlers_.size(); }

 private:
  Logger() noexcept {}
  static std::vector<std::unique_ptr<LogHandler> > handlers_;
  static std::mutex handlers_mutex_;
  static std::atomic<LogLevel> log_level_;

  friend class LogStream;
};

// MACRO for logger

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
    __FILE__ + basename_idx;                                      \
  })

#define LOG(log_level)                   \
  if (log_level < Logger::GetLogLevel()) \
    ;                                    \
  else                                   \
  LogStream(log_level, __FILENAME__, __func__, __LINE__)

#define ASSERT(x)                             \
  if (!(x)) {                                 \
    LOG(ERROR) << "ASSERTION FAIL";           \
    throw std::logic_error("ASSERTION FAIL"); \
  }

class RunTimeLogger : public NonCopyable {
 public:
  RunTimeLogger(const std::string& functor) noexcept
      : begin_time_(std::chrono::high_resolution_clock::now()),
        functor_(functor) {}

  ~RunTimeLogger() noexcept {
    const auto end_time = std::chrono::high_resolution_clock::now();

    std::string time_unit = "ns";
    auto elpased_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        end_time - begin_time_);
    if (elpased_time.count() > 1000000) {
      elpased_time =
          std::chrono::duration_cast<std::chrono::milliseconds>(elpased_time);
      time_unit = "ms";

    } else if (elpased_time.count() > 1000) {
      elpased_time =
          std::chrono::duration_cast<std::chrono::microseconds>(elpased_time);
      time_unit = "us";
    }
    LOG(INFO) << "Run time of " << functor_ << " is " << elpased_time.count()
              << time_unit;
  }

 private:
  const std::chrono::high_resolution_clock::time_point begin_time_;
  const std::string functor_;
};

#define RUN_FUNC RunTimeLogger(__func__);
#define RUN(name) RunTimeLogger(name);
}

#endif /* COLLIE_LOGGER_H_ */
