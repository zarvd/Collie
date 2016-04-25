#include "../inc/logger.h"

namespace collie {
std::atomic<LogLevel> Logger::log_level_(INFO);
}
