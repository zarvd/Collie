#ifndef COLLIE_BASE_EXCEPTION_H
#define COLLIE_BASE_EXCEPTION_H

#include <exception>
#include <stdexcept>

namespace Collie { namespace Base { namespace Exception {

#define EXC_DETAIL Collie::Base::Exception::getDetail(__FILE__, __LINE__, __func__)

inline std::string getDetail(const std::string & file,
                             const unsigned & line,
                             const std::string & func) {
    return file + "::" + func + "::" + std::to_string(line);
}

class RuntimeError : public std::runtime_error {
public:
    RuntimeError() :
        std::runtime_error("Runtime error"), msg("Unknown") {}

    explicit RuntimeError(const std::string & msg) :
        std::runtime_error(msg), msg(msg) {}

    const char* what() const noexcept override {
        return msg.c_str();
    }
protected:
    std::string msg;
};

class EPollPollError : public RuntimeError {
public:
    EPollPollError() : RuntimeError() {}
    explicit EPollPollError(const std::string & method) :
        RuntimeError(method) {}
};

}}}

#endif /* COLLIE_BASE_EXCEPTION_H */
