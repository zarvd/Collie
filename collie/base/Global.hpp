#ifndef COLLIE_BASE_GLOBAL_H
#define COLLIE_BASE_GLOBAL_H

#include "../logging/Logger.hpp"
#include "Exception.hpp"

namespace Collie {

using Logger::LoggingHandler;
using Logger::Level::TRACE;
using Logger::Level::DEBUG;
using Logger::Level::INFO;
using Logger::Level::WARN;
using Logger::Level::ERROR;

enum class Status {FAIL, SUCCESS};

}

#endif /* COLLIE_BASE_GLOBAL_H */
