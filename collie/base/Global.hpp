#ifndef COLLIE_BASE_GLOBAL_H
#define COLLIE_BASE_GLOBAL_H

#include <cstring>
#include "../logging/Logger.hpp"

namespace Collie {

using Logger::LoggingHandler;
using Logger::Level::TRACE;
using Logger::Level::DEBUG;
using Logger::Level::INFO;
using Logger::Level::WARN;
using Logger::Level::ERROR;

enum class Status {FAIL, SUCCESS};

inline std::string getErr() {
    return std::string(strerror(errno));
}

}

#endif /* COLLIE_BASE_GLOBAL_H */
