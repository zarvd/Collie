#ifndef COLLIE_EXCEPTION_H
#define COLLIE_EXCEPTION_H

#include <cstring>
#include <exception>
#include <stdexcept>

namespace Collie { namespace Exception {


inline std::string getErr() {
    return std::string(strerror(errno));
}

inline std::string getWhere(const std::string & file,
                            const unsigned & line,
                            const std::string & func) {
    return file + "(" + std::to_string(line) + "): " + func;
}

class Error : public std::exception {
public:
    explicit Error(const std::string & msg) noexcept : msg(msg) {}
    const char* what() const noexcept override {
        return msg.c_str();
    }

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
    explicit InvalidArgumentError(const std::string & msg) noexcept : Error(msg) {}
};

class SystemError : public Error {
public:
    explicit SystemError(const std::string & msg) noexcept : Error(msg + getErr()) {}
};

class FatalError : public Error {
public:
    explicit FatalError(const std::string & msg) noexcept : Error(msg) {}
};

#define EXC_DETAIL ::Collie::Exception::getWhere(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#define THROW_NOTFOUND throw ::Collie::Exception::NotFoundError(EXC_DETAIL);
#define THROW_OUTOFRANGE throw ::Collie::Exception::OutOfRangeError(EXC_DETAIL);
#define THROW_INVALID_ARGUMENT throw ::Collie::Exception::InvalidArgumentError(EXC_DETAIL);
#define THROW_SYS throw ::Collie::Exception::SystemError(EXC_DETAIL);
#define THROW_FATAL throw ::Collie::Exception::FatalError(EXC_DETAIL);
}}

#endif /* COLLIE_EXCEPTION_H */
