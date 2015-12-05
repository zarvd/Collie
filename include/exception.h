#ifndef COLLIE_EXCEPTION_H
#define COLLIE_EXCEPTION_H

#include <cstring>
#include <exception>
#include <stdexcept>
#include <iostream>

namespace collie {

/**
 * get system error message
 */
inline std::string GetError() { return std::string(strerror(errno)); }

/**
 * format info
 */
inline std::string GetDetail(const std::string& file, const unsigned& line,
                             const std::string& func,
                             const std::string& msg = "") {
  const std::string File = "File: " + file + "(" + std::to_string(line) + ")";
  const std::string Function = "Function : " + func;
  std::string Message;
  if (msg != "") Message = "Message: " + msg;
  return "\n" + File + "\n" + Function + "\n" + Message;
}

class Exception : public std::exception {
 public:
  explicit Exception(const std::string& msg) noexcept : kMsg(msg) {}
  explicit Exception(const char* msg) noexcept : kMsg(msg) {}
  ~Exception() noexcept override {}
  const char* what() const noexcept override { return kMsg.c_str(); }

  const std::string kMsg;
};

#define EXC_DETAIL ::collie::GetDetail(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#define THROW throw ::collie::Exception(EXC_DETAIL);
#define THROW_SYS                                             \
  throw ::collie::Exception(EXC_DETAIL + "\nSystem Error: " + \
                            ::collie::GetError());

#define EXC_DETAIL_(msg) \
  ::collie::GetDetail(__FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define THROW_(msg) throw ::collie::Exception(EXC_DETAIL_(msg));
#define THROW_SYS_(msg)                                             \
  throw ::collie::Exception(EXC_DETAIL_(msg) + "\nSystem Error: " + \
                            ::collie::GetError());

#define REQUIRE(CONDITION)                             \
  if (!(CONDITION)) {                                  \
    throw ::collie::Exception(EXC_DETAIL + "REQUIRE"); \
  }

#define REQUIRE_SYS(CONDITION)                                         \
  if (!(CONDITION)) {                                                  \
    throw ::collie::Exception(EXC_DETAIL + "REQUIRE\nSystem Error: " + \
                              ::collie::GetError());                   \
  }

#define REQUIRE_(CONDITION, msg)                             \
  if (!(CONDITION)) {                                        \
    throw ::collie::Exception(EXC_DETAIL_(msg) + "REQUIRE"); \
  }
}

#endif /* COLLIE_EXCEPTION_H */
