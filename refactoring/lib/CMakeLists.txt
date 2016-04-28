set(BASE_SRC
  timer.cc
  log_stream.cc
  log_handler.cc
  logger.cc
  poll/poller.cc
  poll/epoll_poller.cc
  )

set(NETWORK_SRC
  inet_address.cc
  tcp_stream.cc
  tcp_server.cc
  tcp_client.cc
  socket.cc
  tcp_socket.cc
  )

set(UTIL_SRC
  util/noncopyable.cc
  )

set(LIB_SRC
  ${BASE_SRC}
  ${NETWORK_SRC}
  ${UTIL_SRC})
add_library(collie STATIC ${LIB_SRC})
#install(TARGETS collie DESTINATION /usr/local/lib)
