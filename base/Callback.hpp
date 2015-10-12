#ifndef MINIHTTP_BASE_CALLBACK_HPP
#define MINIHTTP_BASE_CALLBACK_HPP

#include <functional>
#include <memory>
#include "SocketAddress.hpp"


namespace MiniHttp { namespace Base {

typedef std::function<void(const unsigned, std::shared_ptr<SocketAddress>)> AcceptCallback;
typedef std::function<void(const unsigned)> ConnectCallback;
typedef std::function<void(const unsigned)> EventCallback;

}}

#endif /* MINIHTTP_BASE_CALLBACK_HPP */
