set(BASE_SRC
  timer.cc
  logger.cc
  poll/poller.cc
  poll/epoll_poller.cc
  )

set(NETWORK_SRC
  inet_address.cc
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
