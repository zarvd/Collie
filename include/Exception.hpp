#ifndef COLLIE_EXCEPTION_H
#define COLLIE_EXCEPTION_H

#include <cstring>
#include <exception>
#include <stdexcept>

namespace Collie {

/**
 * get system error message
 */
inline std::string
getError() {
    return std::string(strerror(errno));
}

/**
 * format info
 */
inline std::string
getDetail(const std::string & file, const unsigned & line,
          const std::string & func, const std::string & msg = "") {
    const std::string File = "File: " + file + "(" + std::to_string(line) + ")";
    const std::string Function = "Function : " + func;
    const std::string Message = "Message: " + msg;
    return "\n" + File + "\n" + Function + "\n" + Message;
}

class Exception : public std::exception {
public:
    explicit Exception(const std::string & msg) noexcept : msg(msg) {}
    explicit Exception(const char * msg) noexcept : msg(msg) {}
    const char * what() const noexcept override { return msg.c_str(); }

protected:
    std::string msg;
};

#define EXC_DETAIL ::Collie::getDetail(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#define THROW throw ::Collie::Exception(EXC_DETAIL);
#define THROW_SYS                                                              \
    throw ::Collie::Exception(EXC_DETAIL + "\nSystem Error: " + getError());

#define EXC_DETAIL_(msg)                                                       \
    ::Collie::getDetail(__FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define THROW_(msg) throw ::Collie::Exception(EXC_DETAIL_(msg));
#define THROW_SYS_(msg)                                                        \
    throw ::Collie::Exception(EXC_DETAIL_(msg) + "\nSystem Error: " +          \
                              getError());
#define REQUIRE(CONDITION)                                                     \
    if(!(CONDITION)) throw ::Collie::Exception(EXC_DETAIL + "\nREQUIRE");

#define REQUIRE_SYS(CONDITION)                                                 \
    if(!(CONDITION)) THROW_SYS;

#define REQUIRE_(CONDITION, msg)                                               \
    if(!(CONDITION))                                                           \
        throw ::Collie::Exception(EXC_DETAIL_(msg) + "\nREQUIR"                \
                                                     "E");

#define TRACE_LOG Log(TRACE) << "TRACE log";
}

#endif /* COLLIE_EXCEPTION_H */
