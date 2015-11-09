#ifndef COLLIE_EXCEPTION_H
#define COLLIE_EXCEPTION_H

#include <cstring>
#include <exception>
#include <stdexcept>

namespace Collie {
namespace Exception {

/**
 * get system error message
 */
inline std::string
getErr() {
    return std::string(strerror(errno));
}

inline std::string
getDetail(const std::string & file, const unsigned & line,
          const std::string & func, const std::string & msg = "") {
    const std::string File = "File: " + file + "(" + std::to_string(line) + ")";
    const std::string Function = "Function : " + func;
    const std::string Message = "Message: " + msg;
    return "\n" + File + "\n" + Function + "\n" + Message;
}

class Error : public std::exception {
public:
    explicit Error(const std::string & msg) noexcept : msg(msg) {}
    const char * what() const noexcept override { return msg.c_str(); }

protected:
    std::string msg;
};

class NotFoundError : public Error {
public:
    explicit NotFoundError(const std::string & msg) noexcept : Error(msg) {}
};

class OutOfRangeError : public Error {
public:
    explicit OutOfRangeError(const std::string & msg) noexcept : Error(msg) {}
};

class InvalidArgumentError : public Error {
public:
    explicit InvalidArgumentError(const std::string & msg) noexcept
        : Error(msg) {}
};

class SystemError : public Error {
public:
    explicit SystemError(const std::string & msg) noexcept
        : Error(msg + "\nSystemError:" + getErr()) {}
};

class FatalError : public Error {
public:
    explicit FatalError(const std::string & msg) noexcept : Error(msg) {}
};

#define EXC_DETAIL                                                             \
    ::Collie::Exception::getDetail(__FILE__, __LINE__, __PRETTY_FUNCTION__)

#define THROW_NOTFOUND throw ::Collie::Exception::NotFoundError(EXC_DETAIL);
#define THROW_OUTOFRANGE throw ::Collie::Exception::OutOfRangeError(EXC_DETAIL);
#define THROW_INVALID_ARGUMENT                                                 \
    throw ::Collie::Exception::InvalidArgumentError(EXC_DETAIL);
#define THROW_SYS throw ::Collie::Exception::SystemError(EXC_DETAIL);
#define THROW_FATAL throw ::Collie::Exception::FatalError(EXC_DETAIL);

#define EXC_DETAIL_(msg)                                                       \
    ::Collie::Exception::getDetail(__FILE__, __LINE__, __PRETTY_FUNCTION__, msg)
#define THROW_NOTFOUND_(msg)                                                   \
    throw ::Collie::Exception::NotFoundError(EXC_DETAIL_(msg));
#define THROW_OUTOFRANGE_(msg)                                                 \
    throw ::Collie::Exception::OutOfRangeError(EXC_DETAIL_(msg));
#define THROW_INVALID_ARGUMENT_(msg)                                           \
    throw ::Collie::Exception::InvalidArgumentError(EXC_DETAIL_(msg));
#define THROW_SYS_(msg)                                                        \
    throw ::Collie::Exception::SystemError(EXC_DETAIL_(msg));
#define THROW_FATAL_(msg)                                                      \
    throw ::Collie::Exception::FatalError(EXC_DETAIL_(msg));
}
}

#endif /* COLLIE_EXCEPTION_H */
