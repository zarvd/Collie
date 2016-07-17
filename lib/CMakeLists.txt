set(BASE_SRC
  base/String.cc
  base/InetAddress.cc
  base/Logger.cc
  base/LogStream.cc
  base/LogHandler.cc
  # timer.cc
  # poll/poller.cc
  # poll/epoll_poller.cc
  # poll/event_pool.cc
  # poll/event_thread_pool.cc
  )

set(TCP_SRC
  tcp/TCPStream.cc
  tcp/TCPServer.cc
  tcp/TCPClient.cc
  tcp/TCPSocket.cc
  # async_tcp_stream.cc
  # async_tcp_server.cc
  )

set(UTIL_SRC
  util/NonCopyable.cc
  )

set(LIB_SRC
  ${BASE_SRC}
  ${TCP_SRC}
  ${UTIL_SRC})
add_library(collie STATIC ${LIB_SRC})
#install(TARGETS collie DESTINATION /usr/local/lib)
