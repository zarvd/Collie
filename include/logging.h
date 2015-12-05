#ifndef COLLIE_LOGGING_H_
#define COLLIE_LOGGING_H_

// #include <logger/logger.h>
#include "easylogging++.h"

// enable thread safe logging
#define ELPP_THREAD_SAFE

namespace collie {

inline std::string GetSystemError() { return std::string(strerror(errno)); }

inline void ConfigureLogger(
    const std::string& logging_conf_path = "./logging.conf") {
  el::Configurations conf(logging_conf_path);
  el::Loggers::reconfigureAllLoggers(conf);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  el::Loggers::addFlag(el::LoggingFlag::AutoSpacing);
}
}

#define THROW_SYS LOG(FATAL) << ::collie::GetSystemError();
#define CHECK_SYS(CONDITION) CHECK(CONDITION) << ::collie::GetSystemError();

#endif /* COLLIE_LOGGING_H_ */
