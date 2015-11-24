#ifndef COLLIE_LOGGING_H_
#define COLLIE_LOGGING_H_

#include <logger/logger.h>

namespace collie {

using logger::LogLevel::TRACE;
using logger::LogLevel::DEBUG;
using logger::LogLevel::INFO;
using logger::LogLevel::WARN;
using logger::LogLevel::ERROR;
#define TRACE_LOG Log(TRACE) << "TRACE log";
}

#endif /* COLLIE_LOGGING_H_ */
