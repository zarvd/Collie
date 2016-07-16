#ifndef COLLIE_BASE_LOGGER_H_
#define COLLIE_BASE_LOGGER_H_

#include <mutex>
#include <vector>
#include "../collie.h"
#include "LogHandler.h"
#include "LogStream.h"

namespace collie {
namespace base {

// Global logger configuration
class Logger {
 public:
  ~Logger() noexcept;

  static void Init();
  static void SetLevel(LogLevel level) noexcept;
  static LogLevel Level() noexcept;

  template <class Handler>
  static void AddLogHandler() noexcept {
    Handler* handler = new Handler;  // XXX use default constructor
    std::lock_guard<std::mutex> lock(logger.handlers_mutex);
    logger.handlers.push_back(handler);
  }
  static void ClearLogHandler() noexcept;
  static SizeType NumOfHandler() noexcept;
  static void Log(const LogLevel level, const String& msg, const String& file,
                  const String& func, unsigned line) noexcept;

 private:
  Logger() noexcept {}

  static Logger logger;  // singleton object
  std::vector<LogHandler*> handlers;
  std::mutex handlers_mutex;
  std::atomic<LogLevel> log_level;
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

#define LOG(log_level)             \
  if (log_level < Logger::Level()) \
    ;                              \
  else                             \
  LogStream(log_level, __FILENAME__, __func__, __LINE__)

#define ASSERT(x)                             \
  if (!(x)) {                                 \
    LOG(ERROR) << "ASSERTION FAIL";           \
    throw std::logic_error("ASSERTION FAIL"); \
  }

// Run time logger for logging it's life time
class RunTimeLogger : public util::NonCopyable {
 public:
  RunTimeLogger(const String& functor) noexcept
      : begin_time_(std::chrono::high_resolution_clock::now()),
        functor(functor) {}

  ~RunTimeLogger() noexcept {
    const auto end_time = std::chrono::high_resolution_clock::now();

    // Format time unit
    String time_unit = "ns";
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
    LOG(INFO) << "Run time of " << functor << " is " << elpased_time.count()
              << time_unit;
  }

 private:
  const std::chrono::high_resolution_clock::time_point begin_time_;
  const String functor;
};

#define RUN_FUNC RunTimeLogger(__func__);
#define RUN(name) RunTimeLogger(name);
}
}

#endif /* COLLIE_BASE_LOGGER_H_ */
