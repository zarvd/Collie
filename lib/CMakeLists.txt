set(BASE_SRC
  base/String.cc
  base/InetAddress.cc
  base/Logger.cc
  base/LogStream.cc
  base/LogHandler.cc
  # timer.cc
  base/Poller.cc
  base/EPollPoller.cc
  base/EventPool.cc
  base/EventThreadPool.cc
  )

set(TCP_SRC
  tcp/TCPStream.cc
  tcp/TCPServer.cc
  tcp/TCPClient.cc
  tcp/TCPSocket.cc
  tcp/AsyncTCPStream.cc
  tcp/AsyncTCPServer.cc
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
