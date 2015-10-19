#ifndef COLLIE_GLOBAL_H
#define COLLIE_GLOBAL_H

#include <logger/Logger.hpp>
#include "Exception.hpp"

namespace Collie {

using Logger::logger;
using Logger::Level::TRACE;
using Logger::Level::DEBUG;
using Logger::Level::INFO;
using Logger::Level::WARN;
using Logger::Level::ERROR;

enum class IP {V4, V6, Unknown};

}

#endif /* COLLIE_GLOBAL_H */
