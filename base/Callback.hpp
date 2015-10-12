#ifndef MINIHTTP_BASE_CALLBACK_HPP
#define MINIHTTP_BASE_CALLBACK_HPP

#include <functional>


namespace MiniHttp { namespace Base {

typedef std::function<void(const unsigned)> ConnectCallback;
typedef std::function<void(const unsigned)> EventCallback;

}}

#endif /* MINIHTTP_BASE_CALLBACK_HPP */
