#ifndef COLLIE_EXCEPTION_H_
#define COLLIE_EXCEPTION_H_

#include <stdexcept>
#include <string.h>

namespace collie {

class SysException : public std::runtime_error {
 public:
  explicit SysException(const std::string& message) noexcept
      : std::runtime_error(message + ": " + ::strerror(errno)) {}
  ~SysException() noexcept override {}
};

class TcpException : public SysException {
 public:
  explicit TcpException(const std::string& message) noexcept
      : SysException(message) {}
  ~TcpException() noexcept override {}
};

class PollException : public SysException {
 public:
  explicit PollException(const std::string& message) noexcept
      : SysException(message) {}
  ~PollException() noexcept override {}
};
}

#endif /* COLLIE_EXCEPTION_H_ */
